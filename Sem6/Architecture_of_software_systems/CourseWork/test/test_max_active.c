#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(Task0);  DeclareTask(Task1);  DeclareTask(Task2);  DeclareTask(Task3);
DeclareTask(Task4);  DeclareTask(Task5);  DeclareTask(Task6);  DeclareTask(Task7);
DeclareTask(Task8);  DeclareTask(Task9);  DeclareTask(Task10); DeclareTask(Task11);
DeclareTask(Task12); DeclareTask(Task13); DeclareTask(Task14); DeclareTask(Task15);
DeclareTask(MainTask);

volatile int active_count = 0;
volatile int max_active = 0;

#define MAKE_TASK(n) \
TASK(Task##n, 1) { \
  ++active_count; \
  if (active_count > max_active) max_active = active_count; \
  TerminateTask(); \
}

MAKE_TASK(0)  MAKE_TASK(1)  MAKE_TASK(2)  MAKE_TASK(3)
MAKE_TASK(4)  MAKE_TASK(5)  MAKE_TASK(6)  MAKE_TASK(7)
MAKE_TASK(8)  MAKE_TASK(9)  MAKE_TASK(10) MAKE_TASK(11)
MAKE_TASK(12) MAKE_TASK(13) MAKE_TASK(14) MAKE_TASK(15)

TASK(MainTask, 2)
{
  ActivateTask(Task0); ActivateTask(Task1); ActivateTask(Task2); ActivateTask(Task3);
  ActivateTask(Task4); ActivateTask(Task5); ActivateTask(Task6); ActivateTask(Task7);
  ActivateTask(Task8); ActivateTask(Task9); ActivateTask(Task10); ActivateTask(Task11);
  ActivateTask(Task12); ActivateTask(Task13); ActivateTask(Task14); ActivateTask(Task15);
  assert(max_active <= 16);
  printf("Max active tasks test passed (max_active=%d)\n", max_active);
  TerminateTask();
}

int main(void)
{
  StartOS(MainTask);
  return 0;
}
