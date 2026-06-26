#include <stdlib.h>
#include <ucontext.h>
#include "internal.h"

static void TaskEntry(int task_idx)
{
  TCB *tcb = &g_tasks[task_idx];
  tcb->entry();
  TerminateTask();
}

void RegisterTask(TTask task, uint8_t priority)
{
  if (g_task_count >= MAX_TASKS)
    return;
  TCB *tcb = &g_tasks[g_task_count];
  tcb->entry = (void (*)(void))task;
  tcb->priority = priority;
  tcb->base_priority = priority;
  tcb->state = TASK_STATE_SUSPENDED;
  tcb->pending_events = 0;
  tcb->waiting_mask = 0;
  tcb->held_resource = -1;
  tcb->next = NULL;
  tcb->waiting_next = NULL;
  tcb->stack = malloc(8192);
  if (tcb->stack == NULL)
    return;
  getcontext(&tcb->context);
  tcb->context.uc_stack.ss_sp = tcb->stack;
  tcb->context.uc_stack.ss_size = 8192;
  tcb->context.uc_link = &g_main_ctx;
  makecontext(&tcb->context, (void (*)(void))TaskEntry, 1, g_task_count);
  ++g_task_count;
}

void ActivateTask(TTask task)
{
  if (!g_os_running)
    return;
  TCB *target = NULL;
  for (uint8_t i = 0; i < g_task_count; ++i)
  {
    if (g_tasks[i].entry == (void (*)(void))task)
    {
      target = &g_tasks[i];
      break;
    }
  }
  if (target == NULL)
    return;
  if (target->state != TASK_STATE_SUSPENDED)
    return;
  target->state = TASK_STATE_READY;
  ++g_active_task_count;
  AddToReadyQueue(target);
  Schedule();
}

void TerminateTask(void)
{
  if (!g_os_running)
    return;
  for (uint8_t i = 0; i < g_resource_count; ++i)
  {
    if (g_resources[i].owner == g_current_task)
    {
      if (g_resources[i].waiting_queue != NULL)
        PIP_ReleaseRes(i);
      else
        g_resources[i].owner = NULL;
    }
  }
  g_current_task->state = TASK_STATE_SUSPENDED;
  g_current_task->pending_events = 0;
  g_current_task->waiting_mask = 0;
  --g_active_task_count;
  RemoveFromReadyQueue(g_current_task);
  if (g_ready_queue == NULL)
    ShutdownOS();
  else
    Schedule();
}
