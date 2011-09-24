/*
 * main.c
 *
 *  Created on: 08-09-2011
 *      Author: eagle
 */

#include <string.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd1100/lcd.h"
#include "rfm70/rfm70.h"
#include "spi/spi.h"

#include "common.h"
#include "keypad.h"
#include "ui.h"

volatile int8_t waiting;
volatile uint8_t flags;


volatile struct {
	uint8_t master:1;
	uint8_t mainScreen:1;
	uint8_t channel:4;
} workFlags;

uint8_t packetBuf[MAX_PACKET_LEN];

extern volatile keyState input;


void triggerFlash(void);

// radio
ISR(INT0_vect)//, ISR_NOBLOCK)
{
	//waiting = waiting+1%3;
	if (waiting<3)
		waiting++;
	//flags = 1;
	//lcdChar('i',LCD_BLACK);
}


void enableINT1()
{
	EIMSK |= (1<<INT1);
}
// test / camera
ISR(INT1_vect)
{
	if (workFlags.master)
	{
		// send trigger to slaves
	}
	else
	{
		triggerFlash();
	}

	EIMSK &= ~(1<<INT1);
	registerCallback(250,&enableINT1);
}

uint8_t setup(void)
{
	keyInit();
	lcdInit();

	spiMasterInit();
	uint8_t rinit = rInit();

	// IO

	SET(DDR, F_TRIG);
	SET(DDR, F_QUENCH);

	CLR(PORT, F_TRIG);
	CLR(PORT, F_QUENCH);

	SET(DDR, LED);
	CLR(PORT, LED);
// timers
	initTicker();


	// interrupt
	PORTD |= (1<<PD2)|(1<<PD3);
	// INT0 and INT1 active LOW
	EIMSK |= (1<<INT0) | (1<<INT1);

	sei();

	return rinit;
}

void loop(void);
int main()
{
	if (! setup())
	{
		msgBox_P(PSTR("Error"), PSTR("Radio init error."));
		waitForUserInput();
	}

	loop();


	return 1;

}

void loop(void)
{
	lcdCls();

	//displayMainScreen();
	Menu_Show();
	workFlags.mainScreen = 1;
	for(;;)
	{
		if (input.changed)
		{
			input.changed = 0;
			if (workFlags.mainScreen)
			{
				Menu_Show();
				workFlags.mainScreen = 0;
			}
			else
			{

				if (input.down)
				{
					Menu_SelectNext();
				}
				else if (input.up)
				{
					Menu_SelectPrev();
				}
				else if (input.middle)
				{
					Menu_Click();
				}
				else  if (input.left)
				{
					if (!Menu_isTopLevel())
					{
						Menu_Back();
						Menu_Show();
					}
					else
					{
						Menu_Reset();
						displayMainScreen();

					}
				}
			}
		}
	}
}


void triggerFlashClr()
{
	CLR(PORT, F_TRIG);
}

void triggerFlash()
{
	SET(PORT, F_TRIG);
#ifdef DEBUG
	if (!registerCallback(50,&triggerFlashClr) )
		callbackError(PSTR("Flash_callback"));
#else
	registerCallback(10,&triggerFlashClr);
#endif

}


void modeMaster(void)
{
	workFlags.master = 1;
	Menu_Back();
	Menu_Show();
}
void modeSlave(void)
{
	workFlags.master = 0;
	Menu_Back();
	Menu_Show();
}

void setChan1()
{
	workFlags.channel= 1;
	Menu_Back();
	Menu_Show();
}
void setChan2()
{
	workFlags.channel= 2;
	Menu_Back();
	Menu_Show();
}
void setChan3()
{
	workFlags.channel= 3;
	Menu_Back();
	Menu_Show();
}
void setChan4()
{
	workFlags.channel= 4;
	Menu_Back();
	Menu_Show();
}
void setChan5()
{
	workFlags.channel= 5;
	Menu_Back();
	Menu_Show();
}

void setupSlave(uint8_t chan)
{

}


void slaveReset()
{
	//setupSlave(1);
	Menu_Back();
	Menu_Show();
}
void slaveChan5()
{
	setupSlave(5);
	Menu_Back();
	Menu_Show();
}
void slaveChan4()
{
	setupSlave(4);
	Menu_Back();
	Menu_Show();
}
void slaveChan3()
{
	setupSlave(3);
	Menu_Back();
	Menu_Show();
}
void slaveChan2()
{
	setupSlave(2);
	Menu_Back();
	Menu_Show();
}
void slaveChan1()
{
	setupSlave(1);
	Menu_Back();
	Menu_Show();
}

