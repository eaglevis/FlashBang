/*
 * ui.h
 *
 *  Created on: 21-09-2011
 *      Author: eagle
 */

#ifndef UI_H_
#define UI_H_

#define MSGBOX_WIDTH 80
#define MSGBOX_HEIGHT 50


#include <stdbool.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "lcd1100/lcd.h"

typedef void (*menuitemfuncptr)();




struct PROGMEM _menuSetItem
{
	prog_char *text;
	const uint8_t min;
	const uint8_t max;
	const uint8_t base;
	uint8_t * var;
};

struct PROGMEM _menuitem
{
	prog_char *text;
	menuitemfuncptr menuitemfunc;
	const struct _menuitem *parent;
	const struct _menuitem *submenu;
	const struct _menuitem *next;
	const struct _menuSetItem *set;
};

void displayMainScreen(void);

void Menu_Show();
void Menu_SelectNext();
void Menu_SelectPrev();
void Menu_Click();
void Menu_Back();
void Menu_Reset();
bool Menu_isTopLevel();

extern struct _menuitem PROGMEM menu;

void msgBox_P(char* head, char * str);

#define MSGBOX_MARGIN_X ((LCD_X_RES - MSGBOX_WIDTH)/2)
#define MSGBOX_MARGIN_Y ((LCD_Y_RES - MSGBOX_HEIGHT)/2)
#define MSGBOX_LINE_WIDTH (MSGBOX_WIDTH/6)
#define MSGBOX_LINES ((MSGBOX_HEIGHT-12) / 8)

#define MENU_ROWS 5


#endif /* UI_H_ */
