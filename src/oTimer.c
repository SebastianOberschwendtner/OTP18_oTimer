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

unsigned int new_time = 0;
unsigned int old_time = 0;

//variable for state machine
sys_t* system;
digit_t* digit;
time_t* time;
adc_t* input;

/*
 * initialize the system
 */
void init_sys(void)
{
	//init IOs
	init_io();

	//init the systick timer
	init_timer();

	//register memory
	system = ipc_memory_register(sizeof(sys_t),did_SYSTEM);

	//initialize the watchdog timer for a timeout with an interrupt of approx. 8s
	wdt_enable(WDTO_8S);
};
ISR(WDT_vect)
{
	//When the watchdog underflows, shut the system down
	shutdown();
}

/*
 * initialize the state machine.
 * Important: Only initialize the state machine after(!) all other tasks are initialized!
 */
void init_state_machine(void)
{
	//initialize state
	system->state = STATE_USER_INPUT;

	//Get variables of other tasks
	digit 	= ipc_memory_get(did_DIGIT);
	time 	= ipc_memory_get(did_TIME);
	input 	= ipc_memory_get(did_INPUT);

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

/*
 * run the state machine
 */
void state_machine(void)
{
	switch(system->state)
	{
	case STATE_USER_INPUT:
		//Perform actions for current state
		new_time = ((unsigned int)input->value[0]*10);
		new_time += (unsigned int)input->value[1]*300;

		//Pet the watchdog, when there was a change in the input
		if(new_time != old_time)
			wdt_reset();
		old_time = new_time;

		set_time(new_time); 	//Update time input
		time2digit(); 			//Display the time

		//Check for state transitions and perform exit actions
		if(check_event(EVENT_BUTTONPRESSED,STATE_COUNTING_DOWN))
		{
			set_task(TASK_COUNT_DOWN,ACTIVE); 	//Enable the counter task
		}
		break;

	case STATE_COUNTING_UP:
		break;

	case STATE_COUNTING_DOWN:
		//Perform actions for current state
		time2digit();					//Display the counting time
		wdt_reset();					//Reset the watchdog

		//Check for state transitions and perform exit actions
		if(check_event(EVENT_TIME_OVF,STATE_ALARM))
		{
			set_task(TASK_COUNT_DOWN,INACTIVE); 	//Disable the counter task
			set_task(TASK_ALARM,ACTIVE);		//Enable the alarm task
		}
		else if(check_event(EVENT_BUTTONPRESSED,STATE_USER_INPUT))
		{
			set_task(TASK_COUNT_DOWN,INACTIVE); 	//Disable the counter task
		}
		break;

	case STATE_ALARM:
		//Perform actions for current state
		wdt_reset();					//Reset the watchdog
		//check for state transitions and perform exit actions
		if(check_event(EVENT_BUTTONPRESSED,STATE_USER_INPUT))
		{
			set_task(TASK_ALARM,INACTIVE); 		//Disable the alarm task
			set_alarm_out(OFF);					//Disable the alarm outputs
		}
		break;

	default:
		//Return to the user input state
		system->state = STATE_USER_INPUT;
		break;
	}
};

/*
 * Notify the state machine with a specific event
 */
void state_machine_notify(unsigned char event)
{
	//Set the event flag with the corresponding bit flag
	system->event |= event;
};

/*
 * Check whether a specific event occurred and go to the specified state
 * Returns 1 to signal a state change
 */
unsigned char check_event(unsigned char event, unsigned char next_state)
{
	//check for event flags
	if(system->event & event)
	{
		//Clear all event flags
		system->event = 0;
		//Goto next sate
		system->state = next_state;
		return 1;
	}
	else
		return 0;
};

/*
 * display the current time count
 */
void time2digit(void)
{
	//decide whether to show 1 seconds or not
	if(time->hour || (time->minute >19))
	{
		//Do not show 1 seconds
		int2digit(get_time_int()/10);
		set_dp(2);
		if(time->hour)
			set_dot(0,ON);
		else
			set_dot(0,OFF);
	}
	else
	{
		//Do show 1 seconds
		int2digit(get_time_int());
		set_dp(1);
		set_dot(0,OFF);
	}
};

/*
 * execute the alarm, just toggle all output pins
 */
void alarm(void)
{
	digit->show[1] ^= 1;
	digit->show[2] ^= 1;
	digit->show[3] ^= 1;
	set_alarm_out(TOGGLE);
};

/*
 * Set the control pins for the alarm peripherals
 */
void set_alarm_out(unsigned char state)
{
	switch(state)
	{
	case ON:
		PORTC |= PIN_BUZZ | PIN_VIB;
		break;
	case OFF:
		PORTC &= ~(PIN_BUZZ | PIN_VIB);
		break;
	case TOGGLE:
		PORTC ^= PIN_BUZZ | PIN_VIB;
		break;
	default:
		break;
	}
}
