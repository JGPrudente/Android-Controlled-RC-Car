#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdarg.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

unsigned long pulseIns(uint8_t, uint8_t);

long distance;
long count;
char string[10];
char error[] = "Out of range ";

int main() {
	USART_init();
	DDRD = 0b10000100;

	while(1)
	{
		count = pulseIns(PIND3, 1);		
		distance = (count / 2) * 0.0343;
	
		if (distance >= 100)
		{
			USART_putstring(error);
		}
		else
		{
		
			dtostrf(distance, 2, 2, string);
			strcat(string, " cm ");
			USART_putstring(string);
		}
	_delay_ms(50);
	
	}
}


unsigned long pulseIns(uint8_t pin, uint8_t state)
{
	PORTD = 0b10000000;
	_delay_us(2);
	PORTD = 0b10000100;
	_delay_us(10);
	PORTD = 0b10000000;
	unsigned long width = 0;
	unsigned long loops = 0;
	unsigned long maxs = 5000000;

	while ((PIND & (1<<PORTD3)) != 8)
	{
		if (loops++ == maxs)
		return 0;
	}

	while ((PIND & (1<<PORTD3)) == 8)
	{
		if (loops++ == maxs)
		{
			return 1;
		}
		width++;
	}
	return width;
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
