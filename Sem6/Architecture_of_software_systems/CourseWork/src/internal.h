#ifndef INTERNAL_H
#define INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <ucontext.h>
#include "rtos_api.h"

#define MAX_TASKS 32
#define MAX_RESOURCES 16
#define MAX_EVENTS 16
#define MAX_ACTIVE_TASKS 16
#define MAX_HELD_RESOURCES 8

#define TASK_STATE_SUSPENDED 0
#define TASK_STATE_READY 1
#define TASK_STATE_RUNNING 2
#define TASK_STATE_WAITING 3

typedef struct TCB TCB;

struct TCB
{
  void (*entry)(void);
  uint8_t priority;
  uint8_t base_priority;
  uint8_t state;
  TEventMask pending_events;
  TEventMask waiting_mask;
  TResource held_resource;
  TCB *next;
  TCB *waiting_next;
  ucontext_t context;
  char *stack;
};

typedef struct
{
  TCB *owner;
  TCB *waiting_queue;
  uint8_t is_free;
} Resource;

extern TCB g_tasks[MAX_TASKS];
extern Resource g_resources[MAX_RESOURCES];
extern TCB *g_ready_queue;
extern TCB *g_current_task;
extern uint8_t g_os_running;
extern uint8_t g_task_count;
extern uint8_t g_active_task_count;
extern uint8_t g_resource_count;
extern ucontext_t g_main_ctx;

void AddToReadyQueue(TCB *task);
void RemoveFromReadyQueue(TCB *task);
TCB *FindHighestReady(void);
void Schedule(void);

#endif
