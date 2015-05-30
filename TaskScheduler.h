/*
 Copyright (c) 2013-2015 softWEB Production All Right Reserved, https://swp.pt
 This source is subject to the softWEB Production Licensing models.
 All other rights reserved.
 
 THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 PARTICULAR PURPOSE.
 
 
 Title:		Task Scheduler
 Version:	1.0
 Company:	softWEB Production
 Author:	Isaias Lourenco
 Copyright:	softWEB Production © Moerbius, 2015
 License:	MIT
 Website:	https://swp.pt
 
 */

#ifndef __TASKSCHEDULER_H__
#define __TASKSCHEDULER_H__

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

typedef void (*fp)(void);

#define NUM_TASKS 8

class TaskScheduler
{
public:
	TaskScheduler();
	~TaskScheduler();
	void Begin(void (*function1)(), void (*function2)());
	void Run();
	void SetTimers(unsigned int passed_timer1, unsigned int passed_timer2);
	
	volatile char tick_flag;

private:
	void task_dispatch();
	void set_task(char tsk);
	void reset_task(char tsk);

	unsigned int task_timers[NUM_TASKS];
	fp arrayOfFunctions[NUM_TASKS];
	char task_bits;
	char bit_mask[NUM_TASKS];
	unsigned int timer[NUM_TASKS];
};

extern TaskScheduler scheduler;

#endif
