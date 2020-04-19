/*
 * time.c
 *
 *  Created on: 16.04.2020
 *      Author: Sebastian
 */

#include "time.h"

time_t* systime;

/*
 * init time
 */
void init_time(void)
{
	//register memory
	systime = ipc_memory_register(sizeof(time_t),did_TIME);
};

/*
 * Count the time, has to be called every second.
 * Returns 1 when over or underflow occurred
 */
unsigned char time_count(unsigned char direction)
{
	if(direction == UP)
	{
		//Count seconds
		if(systime->second < 59)
		{
			systime->second++;
			return 0;
		}
		//Count minutes
		else if(systime->minute < 59)
		{
			systime->second = 0;
			systime->minute++;
			return 0;
		}
		//Count hours
		else if(systime->hour < 1)
		{
			systime->second = 0;
			systime->minute = 0;
			systime->hour++;
			return 0;
		}
		//Reset count on overflow
		else
		{
			systime->second = 0;
			systime->minute = 0;
			systime->hour   = 0;
			return 1;
		}
	}
	else if(direction == DOWN)
	{
		//Count seconds
		if(systime->second > 0)
		{
			systime->second--;
			return 0;
		}
		//Count minutes
		else if(systime->minute > 0)
		{
			systime->second = 59;
			systime->minute--;
			return 0;
		}
		//Count hours
		else if(systime->hour > 0)
		{
			systime->second = 59;
			systime->minute = 59;
			systime->hour--;
			return 0;
		}
		//Reset count on overflow
		else
		{
			systime->second = 0;
			systime->minute = 0;
			systime->hour   = 0;
			return 1;
		}
	}
	else
		return 1;
};

/*
 * get the digits to display of the current time
 */
unsigned char get_digit_time(unsigned char digitnumber)
{
	switch(digitnumber)
	{
	case 1:
		return systime->hour;
		break;
	case 2:
		return (unsigned char)(systime->minute/10);
		break;
	case 3:
		return systime->minute%10;
		break;
	case 4:
		return systime->second/10;
		break;
	case 5:
		return systime->second%10;
		break;
	default:
		return 0;
		break;
	}
};

/*
 * get the current time as int to directly display the number
 */
unsigned int get_time_int(void)
{
	return (systime->hour*10000)+(systime->minute*100)+(systime->second);
};

/*
 * Set the time specified by number of seconds
 */
void set_time(unsigned int seconds)
{
	//calculate hours
	systime->hour = (unsigned char)(seconds/3600);

	//calculate minutes
	seconds = seconds - systime->hour*3600;
	systime->minute = (unsigned char)(seconds/60);

	//Calculate seconds
	seconds = seconds - systime->minute*60;
	systime->second = (unsigned char)(seconds);
};
