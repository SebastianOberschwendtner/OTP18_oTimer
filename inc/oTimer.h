/*
 * oTimer.h
 *
 *  Created on: 14.04.2020
 *      Author: Sebastian
 */

#ifndef OTIMER_H_
#define OTIMER_H_
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ipc.h"
#include "scheduler.h"
#include "variables.h"
#include "digit.h"
#include "time.h"
#include "input.h"

/*
 * defines for configuration
 */
#define NUMBER_AVERAGES_ADC	20

/*
 * define tasks
 */
#define TASK_DIGIT			TASK0
#define TASK_INPUT			TASK1
#define TASK_BUTTON			TASK2
#define TASK_BLINK			TASK3
#define TASK_COUNT			TASK4

/*
 * Define Pins
 */
#define PIN_MINUTE_SENSE 	(1<<PC0)
#define PIN_HOUR_SENSE		(1<<PC1)
#define PIN_ON				(1<<PC2)
#define PIN_SWITCH			(1<<PC3)
#define PIN_BUZZ			(1<<PC4)
#define PIN_VIB				(1<<PC5)

#define PIN_DOT				(1<<PD0)
#define PIN_PLUS			(1<<PD1)
#define PIN_MINUS			(1<<PD2)
#define PIN_FET_HOUR		(1<<PD3)
#define PIN_FET_MINUTE_H	(1<<PD4)
#define PIN_FET_MINUTE_L	(1<<PD5)
#define PIN_FET_SECONDS		(1<<PD6)
#define PIN_BLACK			(1<<PD7)

#define PIN_A				(1<<PB0)
#define PIN_B				(1<<PB1)
#define PIN_C				(1<<PB2)
#define PIN_D				(1<<PB3)
#define PIN_LATCH			(1<<PB4)
#define PIN_LIGHT_TEST		(1<<PB5)

/*
 * defines for quick register access
 */
#define systick TCNT1

#define DOT1	3
#define DOT2	2
#define DOT3	1
#define	DOT4	0

/*
 * state defines
 */
#define 	OFF		0
#define		ON		1
#define		TOGGLE	3

#define		UP 		0
#define		DOWN	1

//system states
#define		STATE_STARTUP		0
#define		STATE_USER_INPUT	1
#define		STATE_COUNTING_DOWN	2
#define		STATE_COUNTING_UP	3
#define 	STATE_ALARM			4
//Events
#define		EVENT_BUTTONPRESSED	(1<<0)

/*
 * Functional prototypes
 */
void 			init_sys			(void);
void 			init_state_machine	(void);
void 			init_io				(void);
void 			init_timer			(void);
void 			set_interrupts		(unsigned char state);
void			shutdown			(void);
void 			state_machine		(void);
void 			state_machine_notify(unsigned char event);
void 			check_event			(unsigned char event, unsigned char next_state);
void 			time2digit			(void);

#endif /* OTIMER_H_ */
