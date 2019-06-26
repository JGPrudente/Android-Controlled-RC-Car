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

//Receiver and Transmitter - Bluetooth
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

//Ultrasonic Distance Sensor
void trigger();
unsigned long pulseIns();

//Driving Options
void controlMode();
void selfDriveMode();

//Initialize variables
long distance;	
char string[10];
char strings[10];


int main() {
	USART_init();
	DDRD = 0b11000100;
	DDRB = 0b00000110;	

	while(1)
	{
		PORTD = 0b00000000;
		PORTB = 0b00000000;

		if(UDR0 == '2')
		{
			PORTD = 0b01000000;
		}
		
		if(UDR0 == '4')
		{
			PORTD = 0b10000000;
		}

		trigger();
		distance = pulseIns();
		
		if (distance < 5 && distance !=0)
		{
			PORTB = 0b00000010;
		}
		_delay_ms(100);
				
	}
}

unsigned long pulseIns()
{	
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
	return (width / 2) * 0.0343;
}


void USART_init(void){	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_receive(void){	
	return UDR0;
}


void trigger()
{
	PORTD &= ~(1<<PORTD2);
	_delay_us(2);
	PORTD |= (1<<PORTD2);
	_delay_us(10);
	PORTD &= ~(1<<PORTD2);
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
