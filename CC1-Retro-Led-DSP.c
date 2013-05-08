/*
 * CC1_Retro_Led_DSP.c
 *
 * Created: 21.4.2013 8:37:49
 * Author: Tomas  http://web.bxhome.org?source=CC1_Displ_sourcecode
 
 This is "for fun"project of 4 x 7 seg digits multiplexed display. 
 I used an old and small HP multiplexed display used in '70 in very first calculators.
 Data are loaded via serial port and display turn to idle after couple seconds.
	- Serial setting: 1200,8,1
	- Format: <1234> 
 */ 

#define F_CPU 3686400L
#define USART_BAUDRATE 1200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)  //191
#define DISPLAY_MUX_REFRESH 256 - 144		// 100Hz
#define DISPLAY_TIMEOUT_REFRESH 65536 - 3600	
#define DISPLAY_TIMEOUT 5					//seconds

#include <avr/io.h>
#include <avr/interrupt.h>
#include "LED_7Seg.h"

volatile uint8_t uart_buffer[6]; 
volatile uint8_t led_buffer[4];

static uint8_t cathodes[] = {cathode_1, cathode_2, cathode_3, cathode_4};
volatile uint8_t digit = 0;
volatile uint8_t timeout;

//Convert ASCII to 7Segment raw value
uint8_t ascii_to_7seg(uint8_t ascii) {
	uint8_t result = 0;
	switch(ascii) {
		case '0': result = digit_0; break;
		case '1': result = digit_1; break;
		case '2': result = digit_2; break;
		case '3': result = digit_3; break;
		case '4': result = digit_4; break;
		case '5': result = digit_5; break;
		case '6': result = digit_6; break;
		case '7': result = digit_7; break;
		case '8': result = digit_8; break;
		case '9': result = digit_9; break;
		case 'A': case 'a': result = digit_a; break;
		case 'B': case 'b': result = digit_b; break;
		case 'C': case 'c': result = digit_c; break;
		case 'D': case 'd': result = digit_d; break;
		case 'E': case 'e': result = digit_e; break;
		case 'F': case 'f': result = digit_f; break;		
	}
	return result;
}


//Maintain UART RX
ISR (USART_RX_vect) {		
	uint8_t tmp;
	
	for (tmp = 5; tmp > 0; tmp--) uart_buffer[tmp] = uart_buffer[tmp-1];	//shift buffer
	uart_buffer[0] = UDR;
	if ((uart_buffer[5] = '<') && (uart_buffer[0] = '>')) {
		for (tmp = 0; tmp < 4; tmp++) led_buffer[tmp] = ascii_to_7seg(uart_buffer[tmp+1]);
		timeout = DISPLAY_TIMEOUT;
		TCNT1 = DISPLAY_TIMEOUT_REFRESH;
		TIMSK = (1<<TOIE0) | (1<<TOIE1);			//enable display & timeout -> timer overflow interrupt for Timer0
	}	
}	


//Maintain LED multiplex
ISR (TIMER0_OVF_vect) {
	TCNT0 = DISPLAY_MUX_REFRESH;
	digit++;
	digit &= 0x03;	// keep digit value 0-3
	CATHODE_PORT |= (cathode_1 | cathode_2 | cathode_3 | cathode_4);   //cathodes inactive
	LED_PORT = led_buffer[digit];	
	CATHODE_PORT &= ~(cathodes[digit]);		
}	
	
//Maintain LED active time out
ISR (TIMER1_OVF_vect) {
	TCNT1 = DISPLAY_TIMEOUT_REFRESH;
	timeout--;
	if (timeout==0) {
		LED_PORT = 0;
		CATHODE_PORT |= (cathode_1 | cathode_2 | cathode_3 | cathode_4);	//set cathodes inactive		
		TIMSK = (0<<TOIE0);			//disable display -> timer overflow interrupt for Timer0
		//Switch CPU to IDLE mode here (?)
	}	
}
		
int main(void)
{
	//Port setting - 7Seg LED drive
	LED_DDR = 0xFF;		//set output
	LED_PORT = 0;	
	CATHODE_DDR |= (cathode_1 | cathode_2 | cathode_3 | cathode_4);		//set output
	CATHODE_PORT |= (cathode_1 | cathode_2 | cathode_3 | cathode_4);	//set cathodes inactive
	
	//Timer0
	TCCR0A = (0<<COM0A0) | (0<<COM0A1) | (0<<COM0B0) | (0<<COM0B1) | (0<<WGM00) | (0<<WGM01) | (0<<WGM02); //Timer0 mode select
	TCCR0B = (1<<CS02) | (0<<CS01) | (0<<CS00);  //Timer0 select clock pre-scaler  ( ÷256 )
	TCNT0 = DISPLAY_MUX_REFRESH;	//init value
	TIMSK |= (1<<TOIE0);			//enable display -> timer overflow interrupt for Timer0
	
	//Timer1
	TCCR1A = (0<<COM1A0) | (0<<COM1A1) | (0<<COM1B0) | (0<<COM1B1) | (0<<WGM10) | (0<<WGM11) ; //Timer1 mode select
	TCCR1B = (1<<CS12) | (0<<CS11) | (1<<CS10) | (0<<WGM12);  //Timer1 select clock pre-scaler  ( ÷1024 )
	TCNT1 = DISPLAY_TIMEOUT_REFRESH;	//init value
	TIMSK |= (1<<TOIE1);				//enable display -> timer overflow interrupt for Timer1
	
	//UART 
	UBRRH = (unsigned char)(BAUD_PRESCALE>>8);
	UBRRL = (unsigned char)BAUD_PRESCALE;		//set bit rate
	UCSRB = (1<<RXEN) | (1<<RXCIE);				//uart_rx enable, uart_rx interrupt enable
	UCSRC = (0<<USBS) | (3<<UCSZ0);				//8bit, 1 stop bit
			
	//Test pattern
	led_buffer[0] = ascii_to_7seg('1');
	led_buffer[1] = ascii_to_7seg('2');
	led_buffer[2] = ascii_to_7seg('3');
	led_buffer[3] = ascii_to_7seg('4');
	
   
	sei();		//enable interrupt		
	while(1);	//loop forever
}