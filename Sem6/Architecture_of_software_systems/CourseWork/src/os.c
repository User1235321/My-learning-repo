#include <stdlib.h>
#include <ucontext.h>
#include "internal.h"

ucontext_t g_main_ctx;

void StartOS(TTask task)
{
  if (g_os_running)
    return;
  for (uint8_t i = 0; i < g_task_count; ++i)
  {
    if (g_tasks[i].entry == (void (*)(void))task)
    {
      g_current_task = &g_tasks[i];
      g_current_task->state = TASK_STATE_RUNNING;
      break;
    }
  }
  g_os_running = 1;
  swapcontext(&g_main_ctx, &g_current_task->context);
  ShutdownOS();
}

void ShutdownOS(void)
{
  g_os_running = 0;
  exit(0);
}

void AddToReadyQueue(TCB *task)
{
  if (g_ready_queue == NULL)
  {
    g_ready_queue = task;
    task->next = NULL;
    return;
  }
  TCB *prev = NULL;
  TCB *curr = g_ready_queue;
  while (curr != NULL && curr->priority >= task->priority)
  {
    prev = curr;
    curr = curr->next;
  }
  if (prev == NULL)
  {
    task->next = g_ready_queue;
    g_ready_queue = task;
  }
  else
  {
    task->next = curr;
    prev->next = task;
  }
}

void RemoveFromReadyQueue(TCB *task)
{
  if (g_ready_queue == NULL)
    return;
  if (g_ready_queue == task)
  {
    g_ready_queue = task->next;
    task->next = NULL;
    return;
  }
  TCB *prev = g_ready_queue;
  TCB *curr = g_ready_queue->next;
  while (curr != NULL)
  {
    if (curr == task)
    {
      prev->next = curr->next;
      task->next = NULL;
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}

TCB *FindHighestReady(void)
{
  if (g_ready_queue == NULL)
    return g_current_task;
  TCB *highest = g_ready_queue;
  TCB *curr = g_ready_queue->next;
  while (curr != NULL)
  {
    if (curr->priority > highest->priority)
      highest = curr;
    curr = curr->next;
  }
  return highest;
}

void Schedule(void)
{
  TCB *next = FindHighestReady();
  if (next != NULL && next != g_current_task)
  {
    if (g_current_task->state == TASK_STATE_RUNNING)
    {
      g_current_task->state = TASK_STATE_READY;
      AddToReadyQueue(g_current_task);
    }
    next->state = TASK_STATE_RUNNING;
    TCB *old = g_current_task;
    g_current_task = next;
    swapcontext(&old->context, &next->context);
  }
}
