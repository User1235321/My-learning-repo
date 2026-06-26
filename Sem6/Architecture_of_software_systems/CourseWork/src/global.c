#include "internal.h"

TCB g_tasks[MAX_TASKS];
Resource g_resources[MAX_RESOURCES];
TCB *g_ready_queue = NULL;
TCB *g_current_task = NULL;
uint8_t g_os_running = 0;
uint8_t g_task_count = 0;
uint8_t g_active_task_count = 0;
uint8_t g_resource_count = 0;
