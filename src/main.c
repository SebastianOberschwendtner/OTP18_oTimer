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
	sys_init();
	digit_init();
	input_init();
	time_init();
	scheduler_init();

	schedule(TASK_DIGIT,	1	); 	//Run TASK every tick		(every 1ms)
	schedule(TASK_INPUT,	2	);	//Run TASK every 2 ticks 	(every 2ms)
	schedule(TASK_BUTTON,	250	);	//Run TASK every 100 ticks	(every 100ms)
	schedule(TASK_BLINK,	500	);	//Run TASK every 500 ticks	(every 0.5s)
	schedule(TASK_COUNT,	1000);	//Run TASK every 1000 ticks	(every 1s)

	adc_t* main_adc = ipc_memory_get(did_INPUT);

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
			}

			/***************** TASK_INPUT *********************/
			if(run(TASK_INPUT,tickpassed))
			{
				input_sample();
				char2digit(main_adc->value[current_val]);

			}

			/***************** TASK_BUTTON *********************/
			if(run(TASK_BUTTON,tickpassed))
			{
				if(button_pressed)
				{
					button_pressed = 0;
					current_val ^= 1;
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
