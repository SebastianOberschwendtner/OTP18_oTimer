/*
 * main.c
 *
 *  Created on: 14.04.2020
 *      Author: Sebastian
 */
#include "oTimer.h"

extern volatile unsigned char tickpassed;
extern volatile unsigned char button_pressed;

unsigned int count = 0;
unsigned char current_val = 0;

int main(void)
{
	init_sys();
	init_digit();
	init_input();
	init_time();
	init_scheduler();
	//After all tasks are initialized, the state machine can be initialized
	init_state_machine();

	schedule(TASK_DIGIT,	1	); 	//Run TASK every tick		(every 1ms)
	schedule(TASK_INPUT,	2	);	//Run TASK every 2 ticks 	(every 2ms)
	schedule(TASK_BUTTON,	250	);	//Run TASK every 100 ticks	(every 100ms)
	schedule(TASK_BLINK,	500	);	//Run TASK every 500 ticks	(every 0.5s)
	schedule(TASK_COUNT,	1000);	//Run TASK every 1000 ticks	(every 1s)

	set_interrupts(ON);
	set_dot(DOT1,TOGGLE);

	while(1)
	{
		if(tickpassed)
		{
			tickpassed = 0;
			run_scheduler();

			/***************** TASK_DIGIT *********************/
			if(run(TASK_DIGIT,tickpassed))
			{
				digit_draw();
				state_machine();
			}

			/***************** TASK_INPUT *********************/
			if(run(TASK_INPUT,tickpassed))
			{
				input_sample();
			}

			/***************** TASK_BUTTON *********************/
			if(run(TASK_BUTTON,tickpassed))
			{
				if(button_pressed)
				{
					button_pressed = 0;
					state_machine_notify(EVENT_BUTTONPRESSED);
				}
			}

			/***************** TASK_BLINK *********************/
			if(run(TASK_BLINK,tickpassed))
			{
				set_dot(DOT1,TOGGLE);
			}

			/***************** TASK_COUNT *********************/
			if(run(TASK_COUNT,tickpassed))
			{
				time_count(UP);
			}
		}
	}
	return 0;
}
