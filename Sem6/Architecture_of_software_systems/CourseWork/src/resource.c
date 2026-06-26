#include "internal.h"

void InitRes(TResource ResNum)
{
  if (ResNum >= MAX_RESOURCES)
    return;
  g_resources[ResNum].owner = NULL;
  g_resources[ResNum].waiting_queue = NULL;
  g_resources[ResNum].is_free = 1;
  if (ResNum + 1 > g_resource_count)
    g_resource_count = ResNum + 1;
}

static void UpdateOwnerPriority(TCB *owner)
{
  if (owner == NULL)
    return;
  uint8_t max_prio = owner->base_priority;
  TCB *waiting = owner->waiting_next;
  while (waiting != NULL)
  {
    if (waiting->priority > max_prio)
      max_prio = waiting->priority;
    waiting = waiting->waiting_next;
  }
  owner->priority = max_prio;
}

void PIP_GetRes(TResource ResNum)
{
  if (!g_os_running)
    return;
  if (ResNum >= g_resource_count)
    return;
  Resource *res = &g_resources[ResNum];
  if (res->is_free)
  {
    res->owner = g_current_task;
    res->is_free = 0;
    g_current_task->held_resource = ResNum;
    return;
  }
  if (res->owner == g_current_task)
    return;
  if (g_current_task->priority > res->owner->priority)
    res->owner->priority = g_current_task->priority;
  g_current_task->state = TASK_STATE_WAITING;
  RemoveFromReadyQueue(g_current_task);
  TCB *prev = NULL;
  TCB *curr = res->waiting_queue;
  while (curr != NULL && curr->priority > g_current_task->priority)
  {
    prev = curr;
    curr = curr->waiting_next;
  }
  if (prev == NULL)
  {
    g_current_task->waiting_next = res->waiting_queue;
    res->waiting_queue = g_current_task;
  }
  else
  {
    g_current_task->waiting_next = curr;
    prev->waiting_next = g_current_task;
  }
  Schedule();
}

void PIP_ReleaseRes(TResource ResNum)
{
  if (!g_os_running)
    return;
  if (ResNum >= g_resource_count)
    return;
  Resource *res = &g_resources[ResNum];
  if (res->owner != g_current_task)
    return;
  res->owner = NULL;
  g_current_task->held_resource = -1;
  if (res->waiting_queue != NULL)
  {
    TCB *next_task = res->waiting_queue;
    res->waiting_queue = next_task->waiting_next;
    next_task->waiting_next = NULL;
    next_task->state = TASK_STATE_READY;
    res->owner = next_task;
    next_task->held_resource = ResNum;
    AddToReadyQueue(next_task);
    UpdateOwnerPriority(next_task);
  }
  else
  {
    res->is_free = 1;
  }
  UpdateOwnerPriority(g_current_task);
}
