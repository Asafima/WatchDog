#ifndef __TASK_H__
#define __TASK_H__

#include "uid.h"

typedef int(action_func)(void *param);
typedef struct task task_t;

task_t* TaskCreate(action_func *func, size_t interval_in_seconds, void *param);

void TaskDestroy(task_t *task);

int TaskRun(task_t *task);

UID_t TaskGetUID(const task_t *task);

int TaskCompare(const task_t *task, UID_t uid);

time_t TaskGetNextRunTime(const task_t *task);

void TaskUpdateNextRunTime(task_t *task);

#endif /*__TASK_H__*/


