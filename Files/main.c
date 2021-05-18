/*
 * main.c
 *
 *  Created on: Sep 17, 2020
 *      Author: yehia
 */

#include "STD_TYPES.h"

#include <util/delay.h>

#include "DIO_interface.h"
#include "PORT_interface.h"
#include "EXTI_interface.h"
#include "GIE_interface.h"

#include "CLCD_interface.h"

#define GAME	100
#define WIN		250

void INT0_ISR(void);
void INT1_ISR(void);

u8 random(void);

u8 Global_u8YposPlayer1=0, Global_u8YposPlayer2=0;

void main(void)
{
	u8 Local_u8Clear[8] = {0}, Local_u8Ball[8] = {0,0,0,6,6,0,0,0}, Local_u8Player1[8] = {1,1,1,1,1,1,1,1}, Local_u8Player2[8] = {16,16,16,16,16,16,16,16};
	u8 Local_u8Xpos=1, Local_u8Ypos=0, Local_u8Flag=0;

	PORT_voidInit();
	EXTI_voidINT0Init();
	EXTI_voidINT1Init();

	EXTI_u8SetCallBack(&INT0_ISR, EXTI_INT0);
	EXTI_u8SetCallBack(&INT1_ISR, EXTI_INT1);

	GIE_voidEnableGlobal();

	CLCD_voidInit();

	while(1)
	{
		CLCD_voidWriteSpecialCharacter(Local_u8Clear, 3, 0, !(Global_u8YposPlayer1));
		CLCD_voidWriteSpecialCharacter(Local_u8Clear, 3, 15, !(Global_u8YposPlayer2));

		CLCD_voidWriteSpecialCharacter(Local_u8Player1, 0, 0, Global_u8YposPlayer1);
		CLCD_voidWriteSpecialCharacter(Local_u8Player2, 1, 15, Global_u8YposPlayer2);
		CLCD_voidWriteSpecialCharacter(Local_u8Ball, 2, Local_u8Xpos, Local_u8Ypos);
		_delay_ms(GAME);
		CLCD_voidWriteSpecialCharacter(Local_u8Clear, 3, Local_u8Xpos, Local_u8Ypos);

		if(Local_u8Flag==0)
			Local_u8Xpos++;

		else if(Local_u8Flag==1)
			Local_u8Xpos--;

		if(Local_u8Xpos==14)
		{
			if(Local_u8Ypos==Global_u8YposPlayer2)
				Local_u8Flag=1;
			else
			{
				CLCD_voidWriteSpecialCharacter(Local_u8Ball, 2, Local_u8Xpos, Local_u8Ypos);
				_delay_ms(GAME);
				CLCD_voidClearDisplay();
				while(1)
				{
					CLCD_voidGotoXY(1,1);
					CLCD_voidSendString("Player 1 Wins");
					_delay_ms(WIN);
					CLCD_voidClearDisplay();
					_delay_ms(WIN);
				}
			}
		}
		else if(Local_u8Xpos==1)
		{
			if(Local_u8Ypos==Global_u8YposPlayer1)
				Local_u8Flag=0;
			else
			{
				CLCD_voidWriteSpecialCharacter(Local_u8Ball, 2, Local_u8Xpos, Local_u8Ypos);
				_delay_ms(GAME);
				CLCD_voidClearDisplay();
				while(1)
				{
					CLCD_voidGotoXY(1,1);
					CLCD_voidSendString("Player 2 Wins");
					_delay_ms(WIN);
					CLCD_voidClearDisplay();
					_delay_ms(WIN);
				}
			}
		}
		if(Local_u8Xpos>2 && Local_u8Xpos<13)
			Local_u8Ypos = random();
	}
}

u8 random(void)
{
	static u8 Local_u8Counter=0;
	static u16 Local_u16Next = 3251 ;
	Local_u16Next = (u16)((((u32)Local_u16Next * (u32)Local_u16Next) / 100UL ) % 10000UL) ;

	Local_u8Counter++;

	if(Local_u8Counter==50)
	{
		Local_u16Next=3251;
		Local_u8Counter=0;
	}

	return Local_u16Next % 2;
}

void INT0_ISR(void)
{
	if(Global_u8YposPlayer1==0)
		Global_u8YposPlayer1=1;
	else if(Global_u8YposPlayer1==1)
		Global_u8YposPlayer1=0;
}

void INT1_ISR(void)
{
	if(Global_u8YposPlayer2==0)
		Global_u8YposPlayer2=1;
	else if(Global_u8YposPlayer2==1)
		Global_u8YposPlayer2=0;
}
