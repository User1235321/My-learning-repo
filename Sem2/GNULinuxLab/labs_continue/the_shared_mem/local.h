/* The header local.h */

/*  common header file: parent, producer and consumer */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>

#define ROWS 5
#define COLS 3
#define SLOT_LEN 50
#define N_SLOTS 6
struct MEMORY {
	char buffer[N_SLOTS][SLOT_LEN];
	int head, tail;
	};

struct sembuf 
	acquire = { 0, -1, SEM_UNDO},
	release = { 0, 1, SEM_UNDO};

enum {AVAIL_SLOTS, TO_CONSUME};