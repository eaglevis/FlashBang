/*
 * keypad.c
 *
 *  Created on: 21-09-2011
 *      Author: eagle
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "keypad.h"


volatile keyState input;

ISR(PCINT1_vect)
{
	disablePcInt();

	keyState f;

	f.up = !GET(B1);
	f.down = !GET(B2);
	f.left = !GET(B3);
	f.right = !GET(B4);
	f.middle = !GET(B5);

	if (memcmp(input, f, sizeof(keyState)))
	{

		f.changed = 1;
	}
	input = f;
	registerCallback(200, &enablePcInt);
}

void disablePcInt()
{
	PCICR &= ~(1<<PCIE1);
}

void enablePcInt()
{
	PCICR |= (1<<PCIE1);
}

inline void keyInit()
{
	// pullups to vcc
	SET(PORT, B1);
	SET(PORT, B2);
	SET(PORT, B3);
	SET(PORT, B4);
	SET(PORT, B5);


	PCMSK1 = 31; // PC0..4 -  PCINT8..12
	enablePcInt(); // interrupt enable
}


// Use with caution. halts everything UI-centric
void waitForUserInput()
{
	input.changed = 0;
	while (!input.changed)
		;
	input.changed = 0;
}
