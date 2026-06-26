#ifndef RTOS_API_H
#define RTOS_API_H

#include <stdint.h>

typedef void (*TTask)(void);
typedef int TResource;
typedef uint16_t TEventMask;

#define DeclareTask(TaskID) void TaskID(void)

#define TASK(TaskID, prior) \
  void TaskID(void); \
  static void __attribute__((constructor)) __register_##TaskID(void) \
  { \
    extern void RegisterTask(TTask, uint8_t); \
    RegisterTask((TTask)TaskID, prior); \
  } \
  void TaskID(void)

#define DeclareResource(ResourceID, CP) \
  static const TResource ResourceID = __COUNTER__

#define DeclareEvent(EventID) \
  static const TEventMask EventID = (1 << (__COUNTER__ % 16))

void ActivateTask(TTask task);
void TerminateTask(void);
void InitRes(TResource ResNum);
void PIP_GetRes(TResource ResNum);
void PIP_ReleaseRes(TResource ResNum);
void SetEvent(TTask task, TEventMask mask);
void GetEvent(TTask task, TEventMask *event);
void WaitEvent(TEventMask mask);
void ClearEvent(TEventMask mask);
void StartOS(TTask task);
void ShutdownOS(void);

#endif
