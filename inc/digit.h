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
void 			init_digit		(void);
void 			char2digit		(unsigned char data);
void 			int2digit		(unsigned int data);
void 			display_digit	(unsigned char digitnumber);
void 			digit_draw		(void);
void 			set_dot			(unsigned char dot, unsigned char state);
void 			set_dp			(unsigned char place);
void 			set_dp_state	(unsigned char state);

#endif /* DIGIT_H_ */
