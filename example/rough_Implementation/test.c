




/*
	First rough test and implementation of the `DHT_11` library
	*/


#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h> 

#include "USART_D.h"

volatile uint8_t count = 0;
volatile uint8_t val = 0;

volatile static uint8_t temperature, humidity;
volatile static uint32_t temp_nd_hum_data;
volatile static uint16_t timeStamp;
volatile static uint8_t bit_position;
volatile static uint8_t innerCount;
volatile static uint16_t temp;
volatile static uint8_t checksum;

ISR(PCINT1_vect){

	if(count > 2){
		innerCount++;

		if(bit_is_set(PINC, PC0)){
			timeStamp = TCNT1;
		}else{

			temp = TCNT1 - timeStamp;

			uint8_t bit = (temp > 50) ? 1 : 0;

			if(bit_position < 32){

				temp_nd_hum_data = (temp_nd_hum_data << 1) | bit;
			}else if(bit_position < 40){

				checksum = (checksum << 1) | bit;
			}

			bit_position++;
		}
	}

	count++;
}

inline static void startPCINT(void){

	cli();

	temperature = 0;
	humidity = 0;
	temp_nd_hum_data = 0;
	count = 0;
	checksum = 0;

	TCNT1 = 0;
	timeStamp = 0;
	bit_position = 0;
	innerCount = 0;
	temp = 0;


	DDRC |= (1 << PC0);
	PORTC &= ~(1 << PC0);
	_delay_ms(28);

	PORTC |= (1 << PC0);
	_delay_us(25);
	DDRC &= ~(1 << PC0);

	TCCR1B |= (1 << CS11);

	sei();
}

static void parseInfo(){

	humidity = (temp_nd_hum_data >> 24);
	temperature = (temp_nd_hum_data >> 8);

}

static void isrState(){

	cli();

	parseInfo();

	USART_print("DHT responded : %d times.\r\n", count);

	USART_print("	TIMESTAMP = %d\r\n", timeStamp);

	USART_print("	INNER_COUNT = %d\r\n", innerCount);

	USART_print("	BIT_POSITION = %d\r\n", bit_position);

	USART_print("	Main Data Stream = %d\r\n", temp_nd_hum_data);

	USART_print("	Check Sum = %d\r\n", checksum);

	USART_print("	Temperature = %d\r\n", temperature);

	USART_print("	Humidity = %d\r\n", humidity);

	sei();
}

int main(void) {

	// -------- Inits --------- //

	USART_begin();

	USART_print("\r\n===== PCINT TEST =====\r\n");

	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT8);

	DDRC &= ~(1 << PC0);

	_delay_ms(2000);

	sei();

	while (1) {

		cli();

		USART_print("\r\nInside while()\r\n");
		sei();

		                                  /* wait */

		startPCINT();

		_delay_ms(3000);

		isrState();
	}                                 
	                 /* End event loop */
	return 0;                            /* This line is never reached */
}
