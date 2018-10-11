#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define F_CPU 16000000
#include <util/delay.h>
#include "lcd.h"
#include "Touchscreen.h"

#define HIGH 1
#define LOW 0
#define BUFFER 1024
#define BLACK 0x000001

const uint8_t *winstr=(const uint8_t *)"win";

char displayChar = 0;

uint16_t Pad1 = 31, Pad2 = 31;	// paddle positions
uint16_t xc = 64, yc = 32;		// ball position
int m,n;						// touch coordinate
int dx = 0,dy = 0;				// moving vector 
uint16_t s1 = 48,s2 = 48;		// ascii for scores

//use Time 0 and 2 to generate random numbers
void random_number()
{
	uint8_t v = 0;
	uint8_t dx1 = 0,dy1 = 0;
	
	srand(TCNT0);
	switch (rand() % 4) // random direction
	{
		case 0: 
			dx1 = 1;
			dy1 = 1;
			break;
		case 1:
			dx1 = 1;
			dy1 = -1;
			break;
		case 2:
			dx1 = -1;
			dy1 = 1;
			break;
		case 3:
			dx1 = -1;
			dy1 = -1;
			break;
	}
	
	switch (rand()%2) // random speed
	{
		case 0:
			v = 3;
			break;
		case 1:
			v = 2;
			break;
	}
	
	dx = v * dx1;
	dy = v * dy1;
	
}

//set time1 for buzzer
void buzzer()
{
	TCCR1A = (1<<COM1A0);
	TCCR1B |= (1<<CS10) | (1<<WGM12);
}

//set buzzer
void winbuzzer1()
{
	DDRB |= 1<<DDB1;
	PORTB &= ~(1<<PORTB1);
	buzzer();
}

void winbuzzer2()
{
	PORTB &= ~(1<<PORTB1);
	DDRB &= ~(1<<DDB1);
}

//ball movement
void move()
{
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS12);
	//draw the dashed line in the middle
	for (uint8_t xu = 0; xu <= 7; xu++ )
	{
		uint8_t xu2 = xu * 8;
		drawline(buff,64,xu2 + 2,64,xu2 + 6,BLACK);
	}
	
	clearcircle(buff,xc,yc);                                  //clear the old ball
	write_buffer(buff);
	
	if((xc<=7)&(yc>=Pad1-6)&(yc<=Pad1+7)&(dx<0))               //when the ball hit the left paddle
	{
		dx=-dx;
		OCR1A = 600;
		DDRB |= 1<<DDB1;
		PORTB &= ~(1<<PORTB1);
		buzzer();
		_delay_ms(1000);
		PORTB &= ~(1<<PORTB1);
		DDRB &= ~(1<<DDB1);
	}
		
		
	if((xc>=120)&(yc>=Pad2-6)&(yc<=Pad2+7)&(dx>0))             //when the ball hit the right paddle
	{
		dx=-dx;
		OCR1A = 600;
		DDRB |= 1<<DDB1;
		PORTB &= ~(1<<PORTB1);
		buzzer();
		_delay_ms(1000);
		PORTB &= ~(1<<PORTB1);
		DDRB &= ~(1<<DDB1);
	}
		
		
	if((yc>=60)|(yc<=3))                                       //when the ball hit the up and down boundaries
	{
		dy=-dy;
		OCR1A = 60000;
		DDRB |= 1<<DDB1;
		PORTB &= ~(1<<PORTB1);
		buzzer();
		_delay_ms(1000);
		PORTB &= ~(1<<PORTB1);
		DDRB &= ~(1<<DDB1);
		
	}
		
	if(xc<=3)                                                  //when the right player wins
	{
		PORTB |= 1<<PORTB0;
		PORTB |= 1<<PORTB2;                                    //change lights
		
		
		dx=-dx;
		OCR1A = 10;
		DDRB |= 1<<DDB1;
		PORTB &= ~(1<<PORTB1);
		buzzer();
		
		s2++;
		
		random_number();
		
		clearcircle(buff,xc,yc);
		xc = 64;
		yc = 32;
		fillcircle(buff,xc,yc,3,BLACK);
			
		drawchar(buff,57,0,s1);
		drawchar(buff,67,0,s2);                                 //record the scores
		fillrect(buff,2,Pad1 - 4,2,10,1);
		fillrect(buff,124,Pad2 -4,2,10,1);
		write_buffer(buff);
		
		_delay_ms(10000);
		clearcircle(buff,64,32);
		fillrect(buff,2,Pad1 - 4,2,10,1);
		fillrect(buff,124,Pad2 -4,2,10,1);
		write_buffer(buff);
			
		PORTB &= ~(1<<PORTB1);                                  //change lights back
		DDRB &= ~(1<<DDB1);
		PORTB &= ~(1<<PORTB0);
		PORTB &= ~(1<<PORTB2);                                   
	}
		
	if(xc>=124)                                                 //when the left player wins
	{
		PORTB |= 1<<PORTB0;                                     //change lights
		PORTB |= 1<<PORTB2;
		
		dx=-dx;
		OCR1A = 10;
		DDRB |= 1<<DDB1;
		PORTB &= ~(1<<PORTB1);
		buzzer();
		
		s1++;
			
		random_number();
		
		clearcircle(buff,xc,yc);
		xc = 64;
		yc = 32;
		fillcircle(buff,xc,yc,3,BLACK);
		
		drawchar(buff,57,0,s1);                                    //record the scores
		drawchar(buff,67,0,s2);
		fillrect(buff,2,Pad1 - 4,2,10,1);
		fillrect(buff,124,Pad2 -4,2,10,1);                         
		write_buffer(buff);
		_delay_ms(10000);
		clearcircle(buff,64,32);
		fillrect(buff,2,Pad1 - 4,2,10,1);
		fillrect(buff,124,Pad2 -4,2,10,1);
		write_buffer(buff);
		
		PORTB &= ~(1<<PORTB1);                                     //change lights back
		DDRB &= ~(1<<DDB1);
		PORTB &= ~(1<<PORTB0);
		PORTB &= ~(1<<PORTB2);
	}
	
	if (s1 >= 51)                                                  //when the left player gets 3 points and win the game
	{
			
		
		drawchar(buff,57,0,51);
		for (int c1 = 0;c1 <= 1;c1++)                              //light and music
		{
			for (int l1 = 1;l1 <= 6;l1++)
			{
				
				OCR1A = 7648 - 621 * (l1 - 1);
				if (l1 % 2)
				{
					PORTB |= (1<<PORTB0);
				}
				else
				{
					PORTB &= ~(1<<PORTB0);
				}
				winbuzzer1();
				drawstring(buff,18 + 18 * c1,l1, winstr);
				write_buffer(buff);
				_delay_ms(4000);
				winbuzzer2();
			}
		}
		
		random_number();
		
		_delay_ms(20000);
		s1 = 48;
		s2 = 48;                                                      //reset the score
		clearrect(buff,18,8,36,48);
			
		dx = -dx;
		dy = -dy;
		clearcircle(buff,xc,yc);
		xc = 64;
		yc = 32;                                                      //reset the ball
		fillcircle(buff,xc,yc,3,BLACK);
		clearrect(buff,124,1,2,62);
		clearrect(buff,2,1,2,62);
		Pad1 = 31;
		Pad2 = 31;                                                    //reset the paddle
		fillrect(buff,2,Pad1 - 4,2,10,1);
		fillrect(buff,124,Pad2 -4,2,10,1);
		write_buffer(buff);
		_delay_ms(10000);
		clearcircle(buff,64,32);
		write_buffer(buff);
	}
		
	if (s2 >= 51)                                             //when the left player gets 3 points and win the game
	{
			
			
		drawchar(buff,67,0,51);
		for (int c2 = 0;c2 <= 1;c2++)                         //light and music
		{
			for (int l2 = 1;l2 <= 6;l2++)
			{
				OCR1A = 7648 - 621 * (l2 - 1);
				if (l2 % 2)
				{
					PORTB |= (1<<PORTB0);
				} 
				else
				{
					PORTB &= ~(1<<PORTB0);
				}
				winbuzzer1();
				drawstring(buff,74 + 18 * c2,l2, winstr);
				write_buffer(buff);
				_delay_ms(4000);
				winbuzzer2();
			}
		}
			
			
		random_number();
		
		_delay_ms(20000);
		s1 = 48;
		s2 = 48;                                            //reset the score
		clearrect(buff,74,8,36,48);
			
		dx = -dx;
		dy = -dy;
		clearcircle(buff,xc,yc);
		xc = 64;
		yc = 32;                                            //reset the ball
		fillcircle(buff,xc,yc,3,BLACK);
		clearrect(buff,124,1,2,62);
		clearrect(buff,2,1,2,62);
		Pad1 = 31;
		Pad2 = 31;                                          //reset the paddle
		fillrect(buff,2,Pad1 - 4,2,10,1);
		fillrect(buff,124,Pad2 -4,2,10,1);
		write_buffer(buff);
		_delay_ms(10000);
		clearcircle(buff,64,32);
		write_buffer(buff);
	}
		
	drawchar(buff,57,0,s1);
	drawchar(buff,67,0,s2);                                 //print the socres
	
	xc+=dx;                                                 //ball move
	yc+=dy;
	fillcircle(buff,xc,yc,3,BLACK);
	write_buffer(buff);
	_delay_ms(500);
	
	

}

int main(void)
{
	//setting up the gpio for backlight
	DDRD |= 0x80;
	PORTD &= ~0x80;
	PORTD |= 0x00;
	
	DDRB |= 0x05;
	PORTB &= ~0x05;
	PORTB |= 0x00;
	
	DDRB |= 1<<DDB1;
	PORTB &= ~(1<<PORTB1);
	
	setupADC();
	
	TCCR0B |= (1<<CS00);
	TCCR2B |= (1<<CS00);
	
	random_number();
	
	//lcd initialisation
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	_delay_ms(10000);
	clear_buffer(buff);
	int x1 = 0,y1 = 0;
	
	random_number();
	
	drawrect(buff,0,0,128,64,1);
	
	while (1)
	{	
		PORTB &= ~(1<<PORTB1);
		
		move();                                                 //ball move
		
		fillrect(buff,2,Pad1 - 4,2,10,1);
		fillrect(buff,124,Pad2 -4,2,10,1);                      //draw the paddle
		write_buffer(buff);
		
		if(detect())                                            //detect whether the screen is touched
		{
			x1 = touch_screenX();
			y1 = touch_screenY();
			m = 0.169 * x1 - 22.97;
			n = 0.12 * y1 - 36;
			if (n<=5 && n >= 0)                                  //touch outside the LCD screen
			{
				n = 5;
			}
			if (n >=58)                                          //touch outside the LCD screen
			{
				n=58;
			}
			if(m >= 0 && m <= 127 && n >= 5 && n <= 58)          //touch inside the screen
			{
				if (m <= 63)                                     //touch left part
				{
					if (n<=Pad1)                                 //touching place above the paddle position
					{
						clearrect(buff,2,1,2,62);
						Pad1 -= 3;
						fillrect(buff,2,Pad1 - 4,2,10,1);
						write_buffer(buff);
					}
			
					if(n>Pad1)                                   //touching place below the paddle position
					{
						clearrect(buff,2,1,2,62);
						Pad1 += 3;
						fillrect(buff,2,Pad1 - 4,2,10,1);
						write_buffer(buff);
					}
				} 
				else                                             //touch right part
				{
					if (n<=Pad2)                                 //touching place above the paddle position
					{
						clearrect(buff,124,1,2,62);
						Pad2 -= 3;
						fillrect(buff,124,Pad2 - 4,2,10,1);
						write_buffer(buff);
					}
					
					if(n>Pad2)                                   //touching place below the paddle position
					{
						clearrect(buff,124,1,2,62);
						Pad2 += 3;
						fillrect(buff,124,Pad2 - 4,2,10,1);
						write_buffer(buff);
					}
				}
			}
		}
		
		PORTB &= ~(1<<PORTB1);
		write_buffer(buff);
	}
}