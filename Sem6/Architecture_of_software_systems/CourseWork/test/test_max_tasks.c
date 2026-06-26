#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(MainTask);
DeclareTask(T0);  DeclareTask(T1);  DeclareTask(T2);  DeclareTask(T3);
DeclareTask(T4);  DeclareTask(T5);  DeclareTask(T6);  DeclareTask(T7);
DeclareTask(T8);  DeclareTask(T9);  DeclareTask(T10); DeclareTask(T11);
DeclareTask(T12); DeclareTask(T13); DeclareTask(T14); DeclareTask(T15);
DeclareTask(T16); DeclareTask(T17); DeclareTask(T18); DeclareTask(T19);
DeclareTask(T20); DeclareTask(T21); DeclareTask(T22); DeclareTask(T23);
DeclareTask(T24); DeclareTask(T25); DeclareTask(T26); DeclareTask(T27);
DeclareTask(T28); DeclareTask(T29); DeclareTask(T30);

volatile int registered = 0;

#define MAKE_TASK(n) \
TASK(T##n, 1) { ++registered; if (n == 30) { assert(registered == 31); printf("Max tasks test passed (registered=%d)\n", registered); } TerminateTask(); }

MAKE_TASK(0)  MAKE_TASK(1)  MAKE_TASK(2)  MAKE_TASK(3)
MAKE_TASK(4)  MAKE_TASK(5)  MAKE_TASK(6)  MAKE_TASK(7)
MAKE_TASK(8)  MAKE_TASK(9)  MAKE_TASK(10) MAKE_TASK(11)
MAKE_TASK(12) MAKE_TASK(13) MAKE_TASK(14) MAKE_TASK(15)
MAKE_TASK(16) MAKE_TASK(17) MAKE_TASK(18) MAKE_TASK(19)
MAKE_TASK(20) MAKE_TASK(21) MAKE_TASK(22) MAKE_TASK(23)
MAKE_TASK(24) MAKE_TASK(25) MAKE_TASK(26) MAKE_TASK(27)
MAKE_TASK(28) MAKE_TASK(29) MAKE_TASK(30)

TASK(MainTask, 2)
{
  ActivateTask(T0); ActivateTask(T1); ActivateTask(T2); ActivateTask(T3);
  ActivateTask(T4); ActivateTask(T5); ActivateTask(T6); ActivateTask(T7);
  ActivateTask(T8); ActivateTask(T9); ActivateTask(T10); ActivateTask(T11);
  ActivateTask(T12); ActivateTask(T13); ActivateTask(T14); ActivateTask(T15);
  ActivateTask(T16); ActivateTask(T17); ActivateTask(T18); ActivateTask(T19);
  ActivateTask(T20); ActivateTask(T21); ActivateTask(T22); ActivateTask(T23);
  ActivateTask(T24); ActivateTask(T25); ActivateTask(T26); ActivateTask(T27);
  ActivateTask(T28); ActivateTask(T29); ActivateTask(T30);
  TerminateTask();
}

int main(void)
{
  StartOS(MainTask);
  return 0;
}
