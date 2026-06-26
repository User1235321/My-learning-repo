#include <stddef.h>
#include "internal.h"

void SetEvent(TTask task, TEventMask mask)
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
  target->pending_events |= mask;
  if ((target->pending_events & target->waiting_mask) != 0 &&
      target->state == TASK_STATE_WAITING)
  {
    target->state = TASK_STATE_READY;
    target->pending_events &= ~(target->waiting_mask);
    target->waiting_mask = 0;
    AddToReadyQueue(target);
    Schedule();
  }
}

void GetEvent(TTask task, TEventMask *event)
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
  if (target != NULL && event != NULL)
    *event = target->pending_events;
}

void WaitEvent(TEventMask mask)
{
  if (!g_os_running)
    return;
  if ((g_current_task->pending_events & mask) != 0)
  {
    g_current_task->pending_events &= ~mask;
    return;
  }
  g_current_task->waiting_mask = mask;
  g_current_task->state = TASK_STATE_WAITING;
  RemoveFromReadyQueue(g_current_task);
  Schedule();
}

void ClearEvent(TEventMask mask)
{
  if (!g_os_running)
    return;
  g_current_task->pending_events &= ~mask;
}
