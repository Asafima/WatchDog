#define _XOPEN_SOURCE 700 /* kill */
#include <stdlib.h>		  /* atoi */
#include <time.h>		  /* time */
#include <unistd.h>		  /* fork/exec */
#include <stdio.h>		  /* printf */
#include <sys/wait.h>	  /* SIGSEGV, waitpid */

#include "colourmod.h" /* PRINT, RESET */
#include "watchdog.h"

/* Run test with argv[1] = 0 at start */

static void DreamSleep(int sec, int add_on, int colour);
static void TestWD(char **argv);
static void PrintBGCLine(char *msg, int bgc);

int main(int argc, char **argv)
{
	TestWD(argv);

	(void)argc;
	return 0;
}

static void PrintBGCLine(char *msg, int bgc)
{
	PRINT(BOLD, bgc);
	printf("%s", msg);
	RESET(0);
	printf("\n");
}

static void DreamSleep(int sec, int add_on, int colour)
{
	size_t counter = 0;
	time_t now = time(NULL);
	while (time(NULL) < now + sec)
	{
		PRINT(add_on, colour);
		printf("in loop, %ld", counter++);
		RESET(0);
		printf("\n");
		sleep(1);
	}
	return;
}

static void TestWD(char **argv)
{
	pid_t pid;
	int status;

	if (1 != atoi(argv[1]))
	{
		pid = fork();
		if (0 == pid)
		{
			PrintBGCLine("User App started first try...", BG_MAGENTA);
			sleep(2);
			argv[1] = "1";
			WDStart(argv);
			DreamSleep(10, BOLD, BG_BLUE);
			WDStop(20);
			sleep(2);
			printf("Finished, kill did not take place\n");
		}
		else
		{
			sleep(5);
			PrintBGCLine("User app was killed", BG_RED);
			kill(pid, SIGSEGV);
			waitpid(pid, &status, 0);
		}
	}

	else
	{
		printf("\n");
		PrintBGCLine("User app was revived by WD Successfully!", BG_GREEN);
		system("ps -a");
		WDStart(argv);
		sleep(3);
		PrintBGCLine("Killing WD", BG_RED);
		kill(getppid(), SIGINT);
		sleep(3);
		system("ps -a");
		DreamSleep(5, BOLD, BG_BLACK);
		PrintBGCLine("WD alive!", BG_GREEN);
		sleep(2);
		system("ps -a");
		WDStop(20);
		PrintBGCLine("Second time Success!", BG_CYAN);
		sleep(3);
		PRINT(BOLD, FG_RED);
		printf("Check resource & cleanup with vlg!");
		system("ipcs");
	}
}
