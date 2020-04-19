/*
 * digit.c
 *
 *  Created on: 16.04.2020
 *      Author: Sebastian
 */
#include "digit.h"

digit_t* digits;

unsigned char current_digit = 0;

/*
 * Initialize digits
 */
void init_digit(void)
{
	//register memory
	digits = ipc_memory_register(sizeof(digit_t),did_DIGIT);
};

/*
 * write a char to the display
 */
void char2digit(unsigned char data)
{
	//A char can never reach 1000
	digits->value[0] = 0;
	digits->show[0] 	= 0;

	//get the hundreds
	digits->value[1] = data/100;
	digits->show[1] 	= 1;

	//get the tens, subtract the known hundreds and divide by 10
	data = data - (digits->value[1]*100);
	digits->value[2] = data/10;
	digits->show[2] 	= 1;

	//get the ones, subtract the known tens and divide by 10
	data = data - (digits->value[2]*10);
	digits->value[3] = data;
	digits->show[3] 	= 1;
};

/*
 * write an int to the display
 */
void int2digit(unsigned int data)
{
	//get the thousands
	digits->value[0] = (unsigned char)(data/1000);
	digits->show[0]  = digits->value[0];

	//get the hundreds
	data = data - (digits->value[0]*1000);
	digits->value[1] = (unsigned char)(data/100);
	digits->show[1]  = 1;

	//get the tens, subtract the known hundreds and divide by 10
	data = data - (digits->value[1]*100);
	digits->value[2] = (unsigned char)(data/10);
	digits->show[2]  = 1;

	//get the ones, subtract the known tens and divide by 10
	data = data - (digits->value[2]*10);
	digits->value[3] = (unsigned char)(data);
	digits->show[3]  = 1;
};

/*
 * Display the specified digit
 */
void display_digit(unsigned char digitnumber)
{
	//Get the digit to display
	unsigned char digit = digits->value[digitnumber];

	//set the output to the BCD converter, only the first four bits of the port are changed
	PORTB = (PORTB & 0xF0) | (digit & 0x0F);

	//enable the corresponding fet, only the fet pins are changed
	PORTD = (PORTD & 0x07) | (1<<(digitnumber+3));

	//set the state of the dot
	if(digits->dot[digitnumber])
		PORTD |= PIN_DOT;
	else
		PORTD &= ~PIN_DOT;

	//decide whether to show the current digit
	if(digits->show[digitnumber])
		PORTD |= PIN_BLACK;
	else
		PORTD &= ~PIN_BLACK;
};

/*
 * display every digit of the time, multiplexed
 * has to be called multiple time to display all digits
 */
void digit_draw(void)
{
	//display the current digit and increase the digit counter
	display_digit(current_digit++);
	//when all digits are displayed, reset the counter
	if(current_digit>3)
		current_digit = 0;
};

/*
 * set the state of the dots
 */
void set_dot(unsigned char dot, unsigned char state)
{
	switch(state)
	{
	case ON:
		digits->dot[dot] = ON;
		break;
	case OFF:
		digits->dot[dot] = OFF;
		break;
	case TOGGLE:
			digits->dot[dot] ^= ON;
		break;
	default:
		break;
	}
};

/*
 * set the position of the decimal place
 */
void set_dp(unsigned char place)
{
	//Disable the old dot, when the decimal place is changed
	if(place != digits->dp)
	{
		digits->dot[digits->dp] = 0;

		//Set the new active dot
		digits->dp = place;
	}
};

/*
 * set the state of the decimal place
 */
void set_dp_state(unsigned char state)
{
	switch(state)
	{
	case ON:
		digits->dot[digits->dp] = ON;
		break;
	case OFF:
		digits->dot[digits->dp] = OFF;
		break;
	case TOGGLE:
			digits->dot[digits->dp] ^= ON;
		break;
	default:
		break;
	}
};
