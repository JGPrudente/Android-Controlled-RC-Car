#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD = 0b11000000;    //PD6 and PD7 in PORTD are outputs
    DDRB = 0b00000110;	//PB1 and Pb2 in PORTB are outputs

    while (1) 
	{
		/* Uncomment lines to manipulate which wheels to move */
		//PORTD = 0b01000000;   //PD6 is high	moves left wheel backwards
		//PORTB = 0b00000100;	//PB2 is high	moves right wheel forward
		//PORTD = 0b10000000;	//PD7 is high	moves left wheel forward
		//PORTB = 0b00000010;	//PB1 is high	moves right wheel backwards
		
		// Wheels stop
		//PORTD = 0b10000000;	
		//PORTB = 0b00000010;
    }
}

