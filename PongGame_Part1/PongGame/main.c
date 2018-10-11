#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.h"

#define FREQ 16000000
#define BAUD 9600
#define HIGH 1
#define LOW 0
#define BUFFER 1024
#define BLACK 0x000001
#define c 128

char displayChar = 0;

int main(void)
{
	//setting up the gpio for backlight
	DDRD |= 0x80;
	PORTD &= ~0x80;
	PORTD |= 0x00;
	
	DDRB |= 0x05;
	PORTB &= ~0x05;
	PORTB |= 0x00;
	
	//lcd initialisation
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	_delay_ms(10000);
	clear_buffer(buff);
	
	while (1)
	{
		//drawchar(buff,0,0,65);
		
		//setpixel(buff, 0, 10, BLACK) ;
		//clearpixel(buff, 5, 60);
		//drawstring(buff,0,3,c);
		//drawline(buff,127,15,127,10,BLACK);
		//fillrect(buff,0,0,60,30,1);
		//drawrect(buff,0,0,60,30,1);
		//fillcircle(buff,30,15,8,1);
		//drawcircle(buff,60,30,15,1);
		
		
		write_buffer(buff);
		_delay_ms(5000);
		displayChar++;
	}
}

