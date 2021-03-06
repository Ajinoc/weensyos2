#include "schedos-app.h"
#include "x86sync.h"

/*****************************************************************************
 * schedos-1
 *
 *   This tiny application prints red "1"s to the console.
 *   It yields the CPU to the kernel after each "1" using the sys_yield()
 *   system call.  This lets the kernel (schedos-kern.c) pick another
 *   application to run, if it wants.
 *
 *   The other schedos-* processes simply #include this file after defining
 *   PRINTCHAR appropriately.
 *
 *****************************************************************************/

#ifndef PRINTCHAR
#define PRINTCHAR	('1' | 0x0C00)
#endif

// UNCOMMENT THE NEXT LINE TO USE EXERCISE 8 CODE INSTEAD OF EXERCISE 6
//#define __EXERCISE_8__
// Use the following structure to choose between them:
#ifndef __EXERCISE_8__
#define __EXERCISE_6__
#endif


void
start(void)
{
	int i;

	for (i = 0; i < RUNCOUNT; i++) {
		// Write characters to the console, yielding after each one.
		//*cursorpos++ = PRINTCHAR;

		// Atomic syscall version
		#ifdef __EXERCISE_6__
		sys_print(PRINTCHAR);
		#endif

		// Atomic lock version
		#ifdef __EXERCISE_8__
		while (atomic_swap(&lock, 1) != 0)
			continue;

		*cursorpos++ = PRINTCHAR;
		atomic_swap(&lock, 0);
		#endif

		sys_yield();
	}

	/*// Yield forever.
	while (1)
		sys_yield();*/

	sys_exit(0);
}
