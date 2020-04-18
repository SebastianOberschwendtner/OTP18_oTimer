/*
 * time.h
 *
 *  Created on: 16.04.2020
 *      Author: Sebastian
 */

#ifndef TIME_H_
#define TIME_H_
#include "oTimer.h"

/*
 * functional prototypes
 */
void			init_time		(void);
unsigned char	time_count		(unsigned char direction);
unsigned char 	get_digit_time	(unsigned char digitnumber);
unsigned int 	get_time_int	(void);

#endif /* TIME_H_ */
