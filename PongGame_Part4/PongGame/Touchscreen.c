/*
 * Touchscreen.c
 *
 * Created: 2018/9/23 21:08:37
 *  Author: 123
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "Touchscreen.h"

void setupADC()
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));               //prescaler 128
	//ADCSRA |= (1<<ADATE);                                       //ADC Auto Trigger Enable
	ADCSRA |= (1<<ADEN);                                        //ADC Enable
	ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));              //ADC Auto Trigger Source Selections(free running mode)
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
}

uint16_t detect(void)
{	
	ADMUX &= 0xF0;
	ADMUX |= (1<<MUX1) | (1<<MUX0);
	
	DDRC &= 0xF0;
	PORTC &= 0xF0;
	
	DDRC |= (1<<DDC0) | (1<<DDC1);
	DDRC &= ~(1<<DDC2);
	DDRC &= ~(1<<DDC3);
	PORTC &= ~(1<<DDC0);
	PORTC &= ~(1<<DDC1);
	PORTC &= ~(1<<DDC2);
	PORTC |= (1<<DDC3);
	
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	return ADC<500;
	
}

uint16_t touch_screenX(void)
{	
	ADMUX &= 0xF0;
	ADMUX |= (1<<MUX1);
	
	DDRC &= 0xF0;
	PORTC &= 0xF0;
	
	DDRC |= ((1<<DDC0) | (1<<DDC1));
	DDRC &= ~(1<<DDC2);
	DDRC &= ~(1<<DDC3);
	PORTC &= ~(1<<PORTC2);
	PORTC &= ~(1<<PORTC3);
	PORTC |= (1<<PORTC0);
	PORTC &= ~(1<<PORTC1);
	
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	return ADC;
}
uint16_t touch_screenY(void)
{
	ADMUX &= 0xF0;
	
	DDRC &= 0xF0;
	PORTC &= 0xF0;
	
	DDRC |= ((1<<DDC2) | (1<<DDC3));
	DDRC &= ~(1<<DDC0);
	DDRC &= ~(1<<DDC1);
	PORTC &= ~(1<<PORTC0);
	PORTC &= ~(1<<PORTC1);
	PORTC |= (1<<PORTC2);
	PORTC &= ~(1<<PORTC3);
	
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

uint16_t Accelerometer(void)                          //Read the Accelerometer by A4
{
	ADMUX &= 0xF0;
	ADMUX |= (1<<MUX2);
	
	DDRC &= 0xF0;
	PORTC &= 0xF0;
	
	DDRC &= ~(1<<DDC4);
	PORTC &= ~(1<<PORTC4);
	
	DDRC &= ~(1<<DDC0);
	DDRC &= ~(1<<DDC1);
	DDRC &= ~(1<<DDC2);
	DDRC &= ~(1<<DDC3);
	PORTC &= ~(1<<PORTC2);
	PORTC &= ~(1<<PORTC3);
	PORTC &= ~(1<<PORTC0);
	PORTC &= ~(1<<PORTC1);
	
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	return ADC;
}