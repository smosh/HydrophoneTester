/*
 * Hydrophone.c
 *
 *  Created on: Nov 4, 2014
 *      Author: Josh
 */

#include <string.h>
#include <avr/io.h>
#include "Hydrophone.h"
#include <avr/interrupt.h>

volatile int n;
volatile int out[500] = {0};


int *CollectHydrophoneData(int *buf, int size)
{
	// initalize some variables
	ADCSRA |= 1<<ADIF;
	n = 0;

	// enable interupt functionality
	sei();
	ADCSRA |= (1<<ADSC | 1<<ADIE);

	// hang inside a while loop untill ADC capture is finished
	while(n<size) {}

	// Disable interupt functionality
	ADC &= ~1<<ADIE;
	cli();

	// Copy captured data to a register and return
	for(int i = 0; i < size; i++) {
		buf[i] = out[i];
	}

	return buf;
}



ISR(ADC_vect)
{
	out[n] = ADCH;
	n++;
	ADCSRA |= 1<<ADSC;
}


/* ************** LEVEL 1 ****************** */

void Hydrophone_Init()
{
	ADC_Init();
}

void ADC_Init()
{
	//DIDR0 = 0x3E; //Leave only ADC0 active
	ADMUX = 0x60; //AVcc Reference, Left Adjusted, ADC0 Selected
	ADCSRB |= 0x03; //ADTS = 3;
	ADCSRA = 1<<ADEN | 4<<ADPS0; //Enable ADC, 16TimesPrecaler
}
