#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <stddef.h> /* size_t */

/* If an error occurs, return status can be seen by using
 *  "echo $?" command on UNIX enviroment operating system.
 */
enum ret_status
{
    SYS_FUNC_FAIL = -1,
    SUCCESS,
    SEM_ERR,
    MEM_ERR,
    SCHED_ERR,
    FORK_ERR,
    THREAD_ERR,
    SIGNAL_ERR
};

/* DESCRIPTION:
 * Function creates a new watchdog process.
 * To use this function, user must have an ELF "watchdog.out" file
 * in the app path, which can be compiled using wd.sh script.
 * Watchdog saves current process from being terminated,
 * and performs resuscitation for it when needed.
 * Calling WDStart more than once for the same process
 * without using WDStop inbetween, would result in undefined behaviour.
 *
 * PARAMS:
 * command line arguments that are passed to WDStart,
 * will be also passed to the revived process when needed.
 *
 * RETURN:
 * Function has no return value
 *
 */
void WDStart(char **path);

/* DESCRIPTION:
 * Function stops watchdog protection and performs cleanup
 * for system resources or memory allocations that has been
 * used by watchdog.
 *
 * PARAMS:
 * timeout - max time for WDStop to disturb user app runtime.
 *
 * RETURN:
 * Function has no return value
 *
 */
void WDStop(size_t timeout);

/* used for wd identity, should not be changed */
enum who_am_i
{
    USER_APP,
    WD
};
extern int is_wd;

#endif /* __WATCHDOG_H__ */