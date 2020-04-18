/*
 * input.h
 *
 *  Created on: 16.04.2020
 *      Author: Sebastian
 */

#ifndef INPUT_H_
#define INPUT_H_
#include "oTimer.h"

/*
 * defines for quick register access
 */
#define ADC_FINISHED	(ADCSRA & (1<<ADIF))
#define ADC_RUNNING		(ADCSRA & (1<<ADSC))

/*
 * functional protoypes
 */
void 			input_init		(void);
void 			input_set_adc	(unsigned char state);
void 			input_set_pin	(unsigned char pinnumber);
void		 	input_sample	(void);

#endif /* INPUT_H_ */
