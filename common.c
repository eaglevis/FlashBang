/*
 * common.c
 *
 *  Created on: 21-09-2011
 *      Author: eagle
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "lcd1100/lcd.h"
#include "common.h"



#define NUM_TRIGGERS 5

timeTrigger triggers[NUM_TRIGGERS] ;

volatile uint8_t ticker; // 2x256 -1 == 0,511s max


ISR(TIMER0_COMPA_vect)
{
	ticker++;
	checkCallback();
}

void initTicker()
{
	/* Timer 0 - System tick timer */
	TCCR0A |= (1<<WGM01); // CTC
	TCNT0 = 0;
	OCR0A = 125; // 1ms
	TCCR0B |= (1<<CS00) | (1<<CS01); //presc. 64

	TIMSK0 |= (1<<OCIE0A);
	for (uint8_t i = 0; i < NUM_TRIGGERS; ++i)
	{
		triggers[i].function = 0;
	}
}

void checkCallback()
{
	for (uint8_t i = 0; i < NUM_TRIGGERS; ++i)
	{

		if ( (triggers[i].time >= ticker) && (triggers[i].function != 0) )
		{
			(*triggers[i].function)();
			triggers[i].function = 0;
		}

	}
}

uint8_t registerCallback(uint8_t ticks, void* function)
{
	for (uint8_t i = 0; i < NUM_TRIGGERS; ++i)
	{
		if (triggers[i].function == 0)
		{
			triggers[i].function = function;
			triggers[i].time = ticker + ticks;
			return 1;
		}
	}

	return 0;
}
