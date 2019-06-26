#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
//#include "USART_RS232_H_file.h"	/* include USART library */

#define LED PORTB		/* connected LED on PORT pin */

void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

int main(void)
{
	unsigned char Data_in;
	USART_init();	
	DDRD = 0b11000000;    //PD6 and PD7 in PORTD are outputs
	DDRB = 0b00000110;	  //PB1 and PB2 in PORTB are outputs	
	while(1)
	{
		Data_in = USART_receive();	
		
		if(Data_in == '2')
		{
			PORTD = 0b10000000;
			PORTB = 0b00000010;
		}
		else if(Data_in == '4')
		{
			PORTB = 0b00000010;
			PORTD = 0b00000000;
		}
		else if(Data_in == '6')
		{
			PORTD = 0b10000000;
			PORTB = 0b00000000;
		}
		else if(Data_in == '8')
		{
			PORTD = 0b01000000;
			PORTB = 0b00000100;
		}
		else
		{
			PORTD = 0b00000000;
			PORTB = 0b00000000;
		}
		_delay_ms(250);   
	}
}

void USART_init(void){
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_receive(void){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
}