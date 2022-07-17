#define _XOPEN_SOURCE 700 /* struct sigaction */
#include <signal.h>       /* sigaction */
#include <stdlib.h>       /* getenv, setenv */
#include <pthread.h>      /* pthread_create, join */
#include <sys/sem.h>      /* sys v semaphore */
#include <stdatomic.h>    /* atomic_int */
#include <sys/types.h>    /* pid */
#include <sys/wait.h>     /* waitpid */
#include <stdio.h>        /* printf */

#include "watchdog.h"
#include "scheduler.h" /* SchedulerRun */

#define RW_PERMISSION (0666)
#define SEND_INTERVAL (1)
#define CHECK_INTERVAL (5)
#define MIN_SIGNALS (1)
#define CYCLIC (0)
#define EXPECTED_SIGNALS (CHECK_INTERVAL / SEND_INTERVAL)
#define MSG_LEN (31)
#define WD_EXEC "./watchdog.out"

enum sem_status
{
    WAIT = -1,
    POST = 1
};

enum LogDocument_level
{
    INFO,
    WARNING,
    ERROR
};

static pthread_t communication_thread;
static atomic_int signal_tracker = 0;
static atomic_int stop_watchdog = 0;
static pid_t proc_to_sig_to;
static int sem_id;
static scheduler_t *scheduler;
int is_wd = USER_APP;

/* =================================== Static Declerations ============================= */
static void *SchedulerRunFunc(void *scheduler);
static void DefineSigHandler(int sig, void (*handler_func)(int, siginfo_t *, void *));
static int SetUpScheduler(char **argv);
static int ModifySemaphoreValue(int modify, int sem_id);
static void LogDocument(char *msg, size_t level);

/* =================================== Task Functions ================================== */
static int SendSignalToProcess(void *info);
static int CheckLifeSignals(void *info);
static int CheckStopRequests(void *arg);

/* =================================== Signal Handlers ================================= */
static void Sig1Handler(int sig, siginfo_t *info, void *context);
static void Sig2Handler(int sig, siginfo_t *info, void *context);

/* =================================== Revive Funcs ==================================== */
static void ReviveChildProcess(char *argv[], char *path);

/* =================================== Cleanup Funcs ==================================== */
static void CleanSemaphore(void);
static int ExitIfError(int is_error, int err_status, char *msg);

/* =================================== Function Implementation ========================== */

void WDStart(char **path)
{
    sigset_t signal_set;

    /* define signal handlers for SIGUSR1, SIGUSR2 */
    DefineSigHandler(SIGUSR1, Sig1Handler);
    DefineSigHandler(SIGUSR2, Sig2Handler);

    /* add SIGUSR1 & SIGUSR2 to the signal set */
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGUSR1);
    sigaddset(&signal_set, SIGUSR2);

    /* Set up a scheduler for the current process (WD or User app) */
    ExitIfError(SCHED_ERR == SetUpScheduler(path), SCHED_ERR, "Scheduler SetUp Error");

    /* create semaphore or get it's ID, used for sync */
    sem_id = semget(ftok(path[0], 'A'), 1, RW_PERMISSION | IPC_CREAT);
    ExitIfError(SYS_FUNC_FAIL == sem_id, SEM_ERR, "Failed to get semaphore");

    /* if watchdog never existed, no enviroment variable has ever been created
        will enter only for the first creation */
    if (NULL == getenv("WD_ON"))
    {
        setenv("WD_ON", "1", 1);
        proc_to_sig_to = fork();
        ExitIfError(SYS_FUNC_FAIL == proc_to_sig_to, FORK_ERR, "Fork Failed");

        /* child process (WD) -> execute WD */
        if (0 == proc_to_sig_to)
        {
            ExitIfError(SYS_FUNC_FAIL == execv(WD_EXEC, path), FORK_ERR, "Exec failed\n"); /* in watchdog.out do sem_post (line 103) */
        }
        /* parent process -> user app -> setup scheduler & wait for WD */
        else
        {
            ExitIfError(SYS_FUNC_FAIL == ModifySemaphoreValue(WAIT, sem_id), SEM_ERR, "Change Semaphore value error");
        }
    }
    /* enter here if enviroment variable exist -> meaning WD already created
       I am a child who has been revived (WD or User app), saving my parent pid */
    else
    {
        proc_to_sig_to = getppid();
        /* increment semaphore (sem in wait state from previous reviving or creation) */
        ExitIfError(SYS_FUNC_FAIL == ModifySemaphoreValue(POST, sem_id), SEM_ERR, "Change Semaphore value error");
    }

    /* after preperations (scheduler setup, sem value update)
       check who is using WDStart. both only run schedulers */
    if (is_wd)
    {
        SchedulerRunFunc(scheduler);
    }
    else
    {
        /* create communication thread for exchanging signals with WD
            without interrupting user app's main thread */
        ExitIfError(SUCCESS != pthread_create(&communication_thread, NULL, SchedulerRunFunc, scheduler), THREAD_ERR, "Thread Create error");

        /* make sure main user thread blocks signal_set
            makes user app thread immune to SIGUSR1, SIGUSR2 */
        pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
    }
}

void WDStop(size_t timeout)
{
    time_t start = time(NULL);
    LogDocument("Stopping WatchDog", INFO);
    /* This stops user scheduler, causing immediate cleanup (after scheduler run) */
    if (NULL != scheduler)
    {
        SchedulerStop(scheduler);
    }

    /* perform cleanup for semaphore */
    CleanSemaphore();

    /* send SIGUSR2 to WD, make him stop scheduler & destroy it
        keep signaling until getting response (stop_watchdog increments) */
    do
    {
        kill(proc_to_sig_to, SIGUSR2);
    } while (stop_watchdog == 0 && (size_t)(time(NULL) - start) < (size_t)timeout);

    /* make user app wait for communicate thread, for final cleanup
        (in case user app finishes before signaling occurs) */

    pthread_join(communication_thread, NULL);
}

void *SchedulerRunFunc(void *scheduler)
{
    SchedulerRun((scheduler_t *)scheduler);
    SchedulerDestroy((scheduler_t *)scheduler);
    return (NULL);
}

static int SetUpScheduler(char **argv)
{
    LogDocument("Setting Up Scheduler", INFO);
    scheduler = SchedulerCreate();
    if (NULL == scheduler)
    {
        return (SCHED_ERR);
    }
    if (UIDIsSame(badUID, SchedulerAddTask(scheduler, SendSignalToProcess, NULL, SEND_INTERVAL)))
    {
        return (SCHED_ERR);
    }
    if (UIDIsSame(badUID, SchedulerAddTask(scheduler, CheckLifeSignals, argv, CHECK_INTERVAL)))
    {
        return (SCHED_ERR);
    }
    if (UIDIsSame(badUID, SchedulerAddTask(scheduler, CheckStopRequests, NULL, CHECK_INTERVAL)))
    {
        return (SCHED_ERR);
    }

    return (SUCCESS);
}

static void DefineSigHandler(int sig, void (*handler_func)(int, siginfo_t *, void *))
{
    struct sigaction action = {0};

    /* SA_SIGNFO for adding siginfo_t pid of signal's sender */
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = handler_func;

    /*	define user defined signal handler */
    LogDocument("Setting Handler", INFO);
    ExitIfError(0 > sigaction(sig, &action, NULL), SIGNAL_ERR, "Signal definition error");
}

/* 1st task -> exchange signals between communication thread & WD */
static int SendSignalToProcess(void *info)
{
    /* send signal to partner */
    LogDocument("SIGUSR1 sent", INFO);
    kill(proc_to_sig_to, SIGUSR1);
    (void)info;
    return (CYCLIC);
}

/* 2nd task -> making sure that got SIGUSR1 */
static int CheckLifeSignals(void *argv)
{
    if (EXPECTED_SIGNALS > signal_tracker)
    {
        LogDocument("Recieved unexpected amount of signals", WARNING);
    }

    /* enter here only when partner dies, signal_tracker will be increased,
        meaning not enough signals arrive */
    if (MIN_SIGNALS > signal_tracker)
    {
        proc_to_sig_to = fork();
        ExitIfError(SYS_FUNC_FAIL == proc_to_sig_to, FORK_ERR, "Fork Failed");
        if (0 == proc_to_sig_to)
        {
            if (is_wd) /* if i am watchdog, user app is dead */
            {
                LogDocument("Reviving User Process", ERROR);
                ReviveChildProcess((char **)argv, ((char **)argv)[0]); /* user becomes child of WD */
            }
            else /* watchdog is dead */
            {
                LogDocument("Reviving WD Process", ERROR);
                ReviveChildProcess((char **)argv, WD_EXEC); /* WD becomes child of user */
            }
        }
        /* make sure the new parent waits for it's child to setup */
        ExitIfError(SYS_FUNC_FAIL == ModifySemaphoreValue(WAIT, sem_id), SEM_ERR, "Change Semaphore value error");
    }
    /* reset signal_tracker */
    atomic_fetch_sub(&signal_tracker, signal_tracker);

    return (CYCLIC);
}

/* 3d Scheduler task -> checks for SIGUSR2 signals */
static int CheckStopRequests(void *arg)
{
    /* getting SIGUSR2 means user wants to stop_watchdog */
    if (0 < stop_watchdog)
    {
        SchedulerStop(scheduler);
    }
    (void)arg;
    return (CYCLIC);
}

static void Sig1Handler(int sig, siginfo_t *info, void *context)
{
    (void)sig;
    (void)context;

    /* making sure no other process sent the signal except for the partner */
    if (proc_to_sig_to == info->si_pid)
    {
        atomic_fetch_add(&signal_tracker, 1);
    }
}

static void Sig2Handler(int sig, siginfo_t *info, void *context)
{
    (void)sig;
    (void)context;
    (void)info;
    /* making sure no other process sent the signal except for the partner */
    if (proc_to_sig_to == info->si_pid)
    {
        atomic_fetch_add(&stop_watchdog, 1);

        /* SIGUSR2 means user wants to end WD, make sure WD signals back to user */
        kill(proc_to_sig_to, SIGUSR2);
    }
}

static void ReviveChildProcess(char *argv[], char *path)
{
    execv(path, argv);
}

static int ModifySemaphoreValue(int modify, int sem_id)
{
    struct sembuf action = {0};
    action.sem_num = 0;
    action.sem_op = modify;
    return (semop(sem_id, &action, 1));
}

static void CleanSemaphore(void)
{
    LogDocument("Semaphore Cleanup", INFO);
    ExitIfError(SYS_FUNC_FAIL == semctl(sem_id, 0, IPC_RMID), SEM_ERR, "Change Semaphore value error");
}

static int ExitIfError(int is_error, int err_status, char *msg)
{
    if (is_error)
    {
        WDStop(0);
        LogDocument(msg, ERROR);
        exit(err_status);
    }
    return (SUCCESS);
}

static void LogDocument(char *msg, size_t level)
{
    time_t now = time(NULL);
    struct tm *curr_time = localtime(&now);
    char *who = ((is_wd == 1) ? "WD  " : "USER");
    char *type = (level == ERROR) ? "ERROR  " : (level == WARNING) ? "WARNING"
                                                                   : "INFO   ";

    FILE *LogDocument = fopen("Logger.txt", "a");
    if (NULL == LogDocument)
    {
        return;
    }
    fprintf(LogDocument, "[%2d:%2d:%2d] %s | %s | %s\n",
            curr_time->tm_hour, curr_time->tm_min, curr_time->tm_sec, who, type, msg);
    fclose(LogDocument);
}
