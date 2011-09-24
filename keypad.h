/*
 * keypad.h
 *
 *  Created on: 21-09-2011
 *      Author: eagle
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

typedef struct {
	uint8_t up:1;
	uint8_t down:1;
	uint8_t left:1;
	uint8_t right:1;
	uint8_t middle:1;
	uint8_t changed:1;
} keyState;

#define B1 C, 2
#define B2 C, 1
#define B3 C, 3
#define B4 C, 4
#define B5 C, 0

void keyInit(void);
void waitForUserInput(void);

void enablePcInt(void);
void disablePcInt(void);
#endif /* KEYPAD_H_ */
