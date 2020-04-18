/*
 * oTimer.c
 *
 *  Created on: 14.04.2020
 *      Author: Sebastian
 */
#include "oTimer.h"

//Variables to emulate SysTick timer
volatile unsigned char 	tickpassed = 0;

//variable for the button
volatile unsigned char button_pressed = 0;

/*
 * initialize the system
 */
void sys_init(void)
{
	//init IOs
	init_io();
	//init the systick timer
	init_timer();
};

/*
 * initialize the I/Os
 */
void init_io(void)
{
	/*
	 * Set the output directions:
	 * Pin Name				Direction	Initial State
	 * PIN_MINUTE_SENSE 	Input		-
	 * PIN_HOUR_SENSE		Input		-
	 * PIN_ON				Output		1
	 * PIN_SWITCH			Input		-
	 * PIN_BUZZ				Output		0
	 * PIN_VIB				Output		0
	 * PIN_DOT				Output		0
	 * PIN_PLUS				Output		0
	 * PIN_MINUS			Output		0
	 * PIN_FET_HOUR			Output		0
	 * PIN_FET_MINUTE_H		Output		0
	 * PIN_FET_MINUTE_L		Output		0
	 * PIN_FET_SECONDS		Output		0
	 * PIN_BLACK			Output		1
	 * PIN_A				Output		0
	 * PIN_B				Output		0
	 * PIN_C				Output		0
	 * PIN_D				Output		0
	 * PIN_LATCH			Output		1
	 * PIN_LIGHT_TEST		Output		1
	 */
	DDRB = PIN_LIGHT_TEST | PIN_LATCH | PIN_A | PIN_B | PIN_C | PIN_D;
	DDRC = PIN_ON | PIN_BUZZ | PIN_VIB;
	DDRD = PIN_DOT | PIN_PLUS | PIN_MINUS | PIN_FET_HOUR | PIN_FET_MINUTE_H | PIN_FET_MINUTE_L | PIN_FET_SECONDS | PIN_BLACK;

	PORTB = PIN_LIGHT_TEST;
	PORTC = PIN_ON;
	PORTD = PIN_BLACK;

	//Enable pin change interrupt on PCINT11 (PIN_SWITCH)
	PCICR 	= (1<<PCIE1);
	PCMSK1 	= (1<<PCINT11);
};
ISR(PCINT1_vect)
{
	if(PINC & PIN_SWITCH)
		button_pressed = 1;
}

/*
 * initialize the timers
 */
void init_timer(void)
{
	/*
	 * Use TIM1 as systick timer
	 * Mode: 		CTC
	 * Prescaler: 	1
	 * OCR1A:		6999
	 * t_tick:		1ms
	 */
	OCR1A = 6999;
	TCCR1A = 0;
	TCCR1B = (1<<WGM12) | (1<<CS10);
	TCCR1C = 0;
	TIMSK1 = (1<<OCIE1A);
};
ISR(TIMER1_COMPA_vect)
{
	tickpassed++;
};

/*
 * Set global interrupts
 */
void set_interrupts(unsigned char state)
{
	if(state == ON)
		SREG |= (1<<SREG_I);
	else if (state == OFF)
		SREG &= ~(1<<SREG_I);
};

/*
 * turn off the power
 */
void shutdown(void)
{
	PORTC &= ~PIN_ON;
};
