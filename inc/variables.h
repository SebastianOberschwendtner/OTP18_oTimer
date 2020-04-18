/*
 * variables.h
 *
 *  Created on: 15.04.2020
 *      Author: Sebastian
 */

#ifndef VARIABLES_H_
#define VARIABLES_H_

// DIds for ipc
#define did_SYSTEM		0
#define did_DIGIT		1
#define did_INPUT		2
#define did_TIME		3
#define did_SCHEDULER	4

//timestruct for counting
typedef struct
{
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}time_t;


//struct which contains all information of the digits
typedef struct
{
	unsigned char value[4];
	unsigned char dot[4];
	unsigned char show[4];
	unsigned char sign;
}digit_t;

//struct for the values of the adc
typedef struct
{
	unsigned int	buffer;
	unsigned char 	value[2];
	unsigned char 	sample;
}adc_t;


//struct for state machine and system parameters
typedef struct
{
	unsigned char state;
	unsigned char event;
}sys_t;
#endif /* VARIABLES_H_ */
