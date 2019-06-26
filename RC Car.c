//Includes 3 Modes: Controllable, Autonomous, and Hybrid

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void controlMode();
void selfDriveMode();
void hybridMode();

void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

void trigger();
unsigned long pulseIns();
void autos();

long distance;

int main(void)
{
	USART_init();	
	DDRD = 0b11000100;    //PD6 and PD7 in PORTD are outputs
	DDRB = 0b00000110;
	while(1)
	{
		switch(UDR0)
		//switch(UDR0)
		{
			case '1':
			{
				do 
				{
					controlMode();
				} while (UDR0 != '7');
				break;
			}
			case '3':
			{
				do
				{
					selfDriveMode();
				} while (UDR0 != '7');
				break;		
			}
			case '9':
			{
				do
				{
					hybridMode();
				} while (UDR0 != '7');
				break;
			}
			default:
			{
				PORTD = 0b00000000;
				PORTB = 0b00000000;
			}	
		}
	}
}

void hybridMode()
{
	autos();
	switch(UDR0)
	{
		case '2':
		{
			PORTD = 0b10000000;
			PORTB = 0b00000010;
			break;
		}
		case '4':
		{
			PORTB = 0b00000010;
			PORTD = 0b00000000;
			break;
		}
		case '6':
		{
			PORTD = 0b10000000;
			PORTB = 0b00000000;
			break;
		}
		case '8':
		{
			PORTD = 0b01000000;
			PORTB = 0b00000100;
			break;
		}
		default:
		{
			PORTD = 0b00000000;
			PORTB = 0b00000000;
		}
	}
}

void selfDriveMode()
{
	
	PORTD = 0b10000000;
	PORTB = 0b00000010;
		
	trigger();
	distance = pulseIns();
	if (distance < 25 && distance != 0)
	{
		PORTD = 0b00000000;
	}
	_delay_ms(250);		
}

void autos()
{
	trigger();
	distance = pulseIns();
	if (distance < 10 && distance != 0)
	{
		PORTB = 0b00000000;
		PORTD = 0b00000000;
		_delay_ms(250);
		PORTD = 0b01000000;
		PORTB = 0b00000100;
		_delay_ms(250);
		PORTB = 0b00000000;
		PORTD = 0b00000000;
	
	}
	_delay_ms(250);
}


void trigger()
{
	PORTD &= ~(1<<PORTD2);
	_delay_us(2);
	PORTD |= (1<<PORTD2);
	_delay_us(10);
	PORTD &= ~(1<<PORTD2);
}

void controlMode()
{
	switch(USART_receive())
	//switch(UDR0)
	{
		case '2':
		{
			PORTD = 0b10000000;
			PORTB = 0b00000010;
			break;
		}
		case '4':
		{
			PORTB = 0b00000010;
			PORTD = 0b00000000;
			break;
		}
		case '6':
		{
			PORTD = 0b10000000;
			PORTB = 0b00000000;
			break;
		}
		case '8':
		{
			PORTD = 0b01000000;
			PORTB = 0b00000100;
			break;
		}
		default:
		{
			PORTD = 0b00000000;
			PORTB = 0b00000000;
		}
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



