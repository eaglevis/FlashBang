/*
 * ui.c
 *
 *  Created on: 21-09-2011
 *      Author: eagle
 */

#include <avr/io.h>

#include <stddef.h>
#include <string.h>

#include "common.h"

#include "ui.h"

prog_char txtRoot[]="Mode";

prog_char txtMstr[]="Master";
prog_char txtSlv[]="Slave";

prog_char txtReset[]="Reset All";

prog_char txtSlaves[]="Setup slaves";

prog_char txtChanSel[]="Select channel";
prog_char txtChan1[]="Channel 1";
prog_char txtChan2[]="Channel 2";
prog_char txtChan3[]="Channel 3";
prog_char txtChan4[]="Channel 4";
prog_char txtChan5[]="Channel 5";


prog_char txt4[]="Menu4";

struct _menuitem menuRoot;
struct _menuitem menuMode;


struct _menuitem menuSlaveReset PROGMEM = {txtReset, &slaveReset, &menuRoot, 0, 0};
struct _menuitem menuSlaveChan5 PROGMEM = {txtChan5, &slaveChan5, &menuRoot, 0, &menuSlaveReset};
struct _menuitem menuSlaveChan4 PROGMEM = {txtChan4, &slaveChan4, &menuRoot, 0, &menuSlaveChan5};
struct _menuitem menuSlaveChan3 PROGMEM = {txtChan3, &slaveChan3, &menuRoot, 0, &menuSlaveChan4};
struct _menuitem menuSlaveChan2 PROGMEM = {txtChan2, &slaveChan2, &menuRoot, 0, &menuSlaveChan3};
struct _menuitem menuSlaveChan1 PROGMEM = {txtChan1, &slaveChan1, &menuRoot, 0, &menuSlaveChan2};

struct _menuitem menuChan5 PROGMEM = {txtChan5, &setChan5, &menuRoot, 0, 0};
struct _menuitem menuChan4 PROGMEM = {txtChan4, &setChan4, &menuRoot, 0, &menuChan5};
struct _menuitem menuChan3 PROGMEM = {txtChan3, &setChan3, &menuRoot, 0, &menuChan4};
struct _menuitem menuChan2 PROGMEM = {txtChan2, &setChan2, &menuRoot, 0, &menuChan3};
struct _menuitem menuChan1 PROGMEM = {txtChan1, &setChan1, &menuRoot, 0, &menuChan2};

struct _menuitem menuMode1 PROGMEM = {txtSlv, modeSlave, &menuRoot, 0, 0};
struct _menuitem menuMode PROGMEM = {txtMstr, modeMaster, &menuRoot, 0, &menuMode1};

struct _menuitem menu3 PROGMEM = {txt4, 0, &menuRoot, 0, 0};
struct _menuitem menuSlaves PROGMEM = {txtSlaves, 0, &menuRoot, &menuSlaveChan1, &menu3};
struct _menuitem menuChanSel PROGMEM = {txtChanSel, 0, &menuRoot, &menuChan1, &menuSlaves};
struct _menuitem menuRoot PROGMEM = {txtRoot, 0, 0, &menuMode, &menuChanSel};


void msgBox_P(char* head, char * str)
{

	//filled header rect
	lcdRect(MSGBOX_MARGIN_X, MSGBOX_MARGIN_Y, 8, MSGBOX_WIDTH, LCD_BLACK, LCD_FILL_BLACK);
	//white body rect - just clear
	lcdRect(MSGBOX_MARGIN_X,MSGBOX_MARGIN_Y+10,MSGBOX_HEIGHT-8,MSGBOX_WIDTH, LCD_WHITE, LCD_FILL_WHITE);

	lcdStrPos(MSGBOX_MARGIN_X+1,MSGBOX_MARGIN_Y+1);
	lcdStr_P(head, LCD_WHITE);

	char chr;
	uint8_t i = 0;
	while ( (chr=pgm_read_byte(str++)) )
	{
		if ( i / MSGBOX_LINE_WIDTH > MSGBOX_LINES) break;
		if ( ( i % MSGBOX_LINE_WIDTH) == 0)
		{
			//lcdNewLine();
			lcdStrPos(MSGBOX_MARGIN_X+2,(i / MSGBOX_LINE_WIDTH)*8 + MSGBOX_MARGIN_Y+12);
			if (chr == ' ')
				continue; // first char in line == space? skip it
		}
		lcdChar(chr, LCD_BLACK);
		++i;
	}

	//body rect
	lcdRect(MSGBOX_MARGIN_X,MSGBOX_MARGIN_Y+10,MSGBOX_HEIGHT-8,MSGBOX_WIDTH, LCD_BLACK, LCD_FILL_NONE);
	lcdUpdate();
}


#define GetAddr(ptr, field) (void*)pgm_read_word(((uint8_t*)ptr)+offsetof(struct _menuitem, field))

static const struct _menuitem *currMenuPtr=&menuRoot;
static int8_t menuindex;
static int8_t menufirstpos;

const struct _menuitem *GetMenuItem(uint8_t index)
{
	const struct _menuitem *tmpmenuitem=currMenuPtr;

	while((tmpmenuitem) && (index>0))
	{
		tmpmenuitem=GetAddr(tmpmenuitem, next);
		index--;
	}
	return tmpmenuitem;
}

void Menu_Show()
{

	//lcdCls();
	//header
	lcdRect(0, 0, 8, LCD_X_RES, LCD_BLACK, LCD_FILL_BLACK);
	lcdStrPos(20,1);
	lcdStr_P(PSTR("Menu"), LCD_WHITE);

	const struct _menuitem *tmpmenuitem=GetMenuItem(menufirstpos);


	for(uint8_t i=0;i<MENU_ROWS;i++)
	{
		//uint8_t charcnt=1;

		lcdRect(0, i*10+10, 10, LCD_X_RES, LCD_BLACK, ((i+menufirstpos)==menuindex) ? LCD_FILL_BLACK: LCD_FILL_WHITE);
		lcdStrPos(5,(i*10)+12);
		if (tmpmenuitem)
		{
			if(GetAddr(tmpmenuitem, text))
			{
				lcdStr_P(GetAddr(tmpmenuitem, text), ((i+menufirstpos)==menuindex) ? LCD_WHITE : LCD_BLACK);
				//charcnt+=strlen_P(GetAddr(tmpmenuitem, text));
			}
			if(GetAddr(tmpmenuitem, submenu))
			{
				lcdStrPos(LCD_X_RES-7,(i*10)+12);
				lcdChar('>', ((i+menufirstpos)==menuindex) ? LCD_WHITE : LCD_BLACK);
				//charcnt++;
			}
			//for(;charcnt<LCD_COLUMNS;charcnt++) lcd_putchar(' '); //Wyczyæ resztê wywietlanej linii
			tmpmenuitem=GetAddr(tmpmenuitem, next);
		}

	}
	lcdUpdate();
}

void Menu_SelectNext()
{
	if(GetMenuItem(menuindex+1)!=0)
	{
		menuindex++;
		if((menuindex-menufirstpos)>=MENU_ROWS) menufirstpos++;
	} else
	{
		menuindex=0;
		menufirstpos=0;
	}
	Menu_Show();
}

void Menu_SelectPrev()
{
	if(menuindex>0)
	{
		menuindex--;
		if(menuindex<menufirstpos) menufirstpos--;
	} else
	{
		const struct _menuitem *tmpmenuitem=currMenuPtr;
		while(tmpmenuitem)
		{
			tmpmenuitem=GetAddr(tmpmenuitem,next);
			if(tmpmenuitem) menuindex++;
		}
		menufirstpos=menuindex-1;
		if(menufirstpos<0) menufirstpos=0;
	}
	Menu_Show();
}

void Menu_Back()
{
	const struct _menuitem *tmpMenuPtr=GetAddr(currMenuPtr, parent);
	if (tmpMenuPtr)
	{
		menufirstpos=0;
		menuindex=0;
		currMenuPtr=tmpMenuPtr;
	}
}

void Menu_Click()
{
	const struct _menuitem *tmpmenuitem=GetMenuItem(menuindex);
	const struct _menuitem *submenu=GetAddr(tmpmenuitem, submenu);

	menuitemfuncptr mfptr=GetAddr(tmpmenuitem, menuitemfunc);
	if(mfptr) (*mfptr)();
	if(submenu)
	{
		currMenuPtr=submenu;
		menuindex=0;
		menufirstpos=0;
	}
	Menu_Show();
}

void Menu_Reset()
{
	currMenuPtr=&menuRoot;
	menuindex=0;
	menufirstpos=0;
}

inline bool Menu_isTopLevel()
{
	return !(bool)GetAddr(currMenuPtr, parent);
}


void displayMainScreen()
{
	lcdCls();
	lcdRect(LCD_X_RES-31,0,30,31,LCD_BLACK, LCD_FILL_BLACK);
	lcdStrPos(LCD_X_RES-30,1);
	lcdStr_P(PSTR("Chan."), LCD_WHITE);

	lcdUpdate();
}
