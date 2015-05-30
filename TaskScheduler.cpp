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

#include "TaskScheduler.h"

TaskScheduler::TaskScheduler() {
	bit_mask[0] = 1;
	bit_mask[1] = 2;
	bit_mask[2] = 4;
	bit_mask[3] = 8;
	bit_mask[4] = 16;
	bit_mask[5] = 32;
	bit_mask[6] = 64;
	bit_mask[7] = 128;
	
	task_bits = 0;
	
	for (int i = 0; i < NUM_TASKS; i++)
	{
		task_timers[i] = 0;
	}
	
}

TaskScheduler::~TaskScheduler() {
}

void TaskScheduler::Begin(void (*function1)(), void (*function2)()) {
	cli();			//disable all interrupts
	
	arrayOfFunctions[0] = function1;
	arrayOfFunctions[1] = function2;
	
	TCCR0B = 0x00;	//stop
	TCNT0 = 0x00;	//set count
	TCCR0A = 0x02;	//CTC mode
	OCR0A = 0x9C;
	TCCR0B = 0x05;	//start timer
	
	MCUCR = 0x00;
	EICRA = 0x00;	//extended ext ints
	EIMSK = 0x00;
	TIMSK0 = 0x02;	//timer 0 interrupt sources
	PRR = 0x00;		//power controller
	
	sei();			//re-enable interrupts
	
	(*arrayOfFunctions[0])();
	reset_task(0);
	(*arrayOfFunctions[1])();
	reset_task(1);
}

void TaskScheduler::Run() {
	if (tick_flag)
	{
		tick_flag = 0;
		task_dispatch();	// well....
	}
}

void TaskScheduler::SetTimers(unsigned int passed_timer1, unsigned int passed_timer2) {
	timer[0] = passed_timer1 / 10;
	timer[1] = passed_timer2 / 10;
	
	task_timers[0] = timer[0];
	task_timers[1] = timer[1];
}

void TaskScheduler::task_dispatch() {
    /* scan the task bits for an active task and execute it */
 
    int task;
      
    /* take care of the task timers. if the value ==0 skip it
    else decrement it. If it decrements to zero, activate the task associated with it */
 
    task = 0;
    while (task < NUM_TASKS ) {
		if (task_timers[task]) {
			task_timers[task]--;		/* dec the timer */
			if (task_timers[task] == 0 ) {
					set_task(task); /* if ==0 activate the task bit */
			}
		}
		task++;
    }
 
    task = 0; /* start at the most significant task */
    while (task <= NUM_TASKS ) {
        if ((task_bits & pgm_read_byte(&bit_mask[task]))) {
            break;	/* if activate task found..*/
        }
        task++;     /* else try the next one */
    }

    switch(task)	
    {
        case 0:
            (*arrayOfFunctions[0])();
            task_timers[0] = timer[0];
            reset_task(0);
            break;
        case 1:
            (*arrayOfFunctions[1])();
            task_timers[1] = timer[1];
            reset_task(1);
            break;
        case 2:
            //task2();
            break;
        case 3:
            //task3();
            break;
        case 4:
            //task4();
            break;
        case 5:
            //task5();
            break;
        case 6:
            //task6();
            break;
        case 7:
            //task7();
            break;
        default:
            break;		
    }
}

void TaskScheduler::set_task(char tsk) {
	task_bits |= pgm_read_byte(&bit_mask[(int)tsk]);		/* sets a task bit */
}

void TaskScheduler::reset_task(char tsk) {
	task_bits &= (~pgm_read_byte(&bit_mask[(int)tsk]));	/* resets a task bit */
}

ISR(TIMER0_COMPA_vect) {
	////TIMER0 has overflowed
 	scheduler.tick_flag = 1;
}

TaskScheduler scheduler;
