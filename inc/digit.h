/*
 * digit.h
 *
 *  Created on: 16.04.2020
 *      Author: Sebastian
 */

#ifndef DIGIT_H_
#define DIGIT_H_
#include "oTimer.h"

/*
 * functional prototypes
 */
void 			time_init		(void);
void 			digit_init		(void);
void 			char2digit		(unsigned char data);
void 			int2digit		(unsigned int data);
void 			display_digit	(unsigned char digitnumber);
void 			digit_draw		(void);
void 			set_dot			(unsigned char dot, unsigned char state);



#endif /* DIGIT_H_ */
