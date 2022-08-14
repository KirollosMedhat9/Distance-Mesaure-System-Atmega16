/*
 ======================================================================================
 Name        : Mini_Project4.c
 Author      : Kirollos Medhat Massoud
 Description : Application File for Mini_Project 4
 Date        : 10/10/2021
 ======================================================================================
 */


#include "ultrasonic.h"
#include <avr/io.h>
#include "lcd.h"

int main(void)
{
	/*Enabling Interrupts module for use of ICU*/
	SREG |= (1<<7);
	/*LCD and Ultrasonic modules initializing*/
	LCD_init();
	Ultrasonic_init();

	LCD_displayString("Distance =");
	/*Variable to save the distance from the Ultrasonic sensor*/
	uint16 distance;
	LCD_displayStringRowColumn(0, 13, "cm");

	while(1){

		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0, 10);
		/*To clear the extra digit while decrementing from a 3 digit number to a 2 digit number*/
		if(distance >= 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			LCD_displayCharacter(' ');
		}


	}
	return 0;
}
