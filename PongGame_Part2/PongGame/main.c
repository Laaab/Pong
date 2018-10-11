#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.h"
#include "uart.h"
#include "Touchscreen.h"

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
	uart_init();
	//setting up the gpio for backlight
	DDRD |= 0x80;
	PORTD &= ~0x80;
	PORTD |= 0x00;
	
	DDRB |= 0x05;
	PORTB &= ~0x05;
	PORTB |= 0x00;
	
	
	setupADC();
	
	
	//lcd initialisation
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	_delay_ms(10000);
	clear_buffer(buff);
	uint16_t x1 = 0,y1 = 0,x2 = 0,y2 = 0;
	
	uint16_t m,n;
	while (1)
	{		
		
		while(!(detect()));                           //detect whether the screen is touched
		x1 = touch_screenX();                         //record ADC about x coordinate
		y1 = touch_screenY();                         //record ADC about y coordinate
		m = 0.169 * x1 - 22.97;                       //Calibrate x to LCD coordinate
		n = 0.12 * y1 - 36;                           //Calibrate y to LCD coordinate
		setpixel(buff,m,n,BLACK);                     //draw pixels by calibrated coordinates
		
		
		write_buffer(buff);
		

	}
}

