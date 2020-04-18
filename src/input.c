/*
 * input.c
 *
 *  Created on: 16.04.2020
 *      Author: Sebastian
 */

#include "input.h"

adc_t* adc;

/*
 * initialize the input peripherals
 */
void input_init(void)
{
	//Select AREF as reference voltage, left adjusted result
	ADMUX = (1<<ADLAR);

	//Select prescaler for ADC: 128
	ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

	//Disable the digital input buffer for PC0 and PC1
	DIDR0 = (1<<ADC1D) | (1<<ADC0D);

	//Enable the ADC
	input_set_adc(ON);

	//Register the memory
	adc = ipc_memory_register(sizeof(adc_t),did_INPUT);

	//intialize the adc buffer
	adc->buffer	 	= 0;
	adc->value[0] 	= 0;
	adc->value[1]	= 0;
	adc->sample 	= 0;
};

/*
 * Set the state of the ADC
 */
void input_set_adc(unsigned char state)
{
	if(state == ON)
		ADCSRA |= (1<<ADEN);
	else if(state == OFF)
		ADCSRA &= ~(1<<ADEN);
};
/*
 * Set the input to be sampled
 */
void input_set_pin(unsigned char pinnumber)
{
	ADMUX = (ADMUX & 0xE0) | (pinnumber & 0x0F);
};

/*
 * perform adc conversion, has to be called twice.
 * the first time the conversion is started, the second time the result is returned.
 * After that every call gives the last conversion result.
 */
void input_sample(void)
{
	//read the result, when it s available
	if(ADC_FINISHED)
	{
		//clear the flag and read the result
		ADCSRA |= (1<<ADIF);

		//update the buffer and increase the sample count
		//only the 8-bit mode of the adc is used.
		adc->buffer += ADCH;
		adc->sample++;

		//when the target number of samples is reached, update the value
		if(adc->sample >= NUMBER_AVERAGES_ADC)
		{
			adc->sample = 0;
			adc->buffer /= NUMBER_AVERAGES_ADC;

			//check which input was sampled
			if(ADMUX & 1)
			{
				adc->value[1] = (((unsigned char)adc->buffer)>>3); //Divide by 8, to get 31 poti positions
				input_set_pin(0);
			}
			else
			{
				adc->value[0] = (((unsigned char)adc->buffer)>>3); //Divide by 8, to get 31 poti positions
				input_set_pin(1);
			}
			adc->buffer = 0;
		}
	}

	//when no conversion is ongoing, start a conversion
	if(~ADC_RUNNING)
		ADCSRA |= (1<<ADSC);
};
