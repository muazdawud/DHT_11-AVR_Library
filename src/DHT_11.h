/*
 * DHT_11 Library - High-performance DHT11 communication interface
 * * Copyright (c) 2026 Dauda Muazu Sulaiman
 * * Copyright (c) 2026 KibrisOrder                <https://ss.kibrisorder.com>
 * * Author: Dauda Muazu Sulaiman
 * * Organization: KibrisOrder
 * * All rights reserved.
 * * This software is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */




/*
	Currently supports all Modern AVR like AtMega328p, 168, 2560 e.t.c

	Doesn't support legacy AVR as they do not have PORT_vect PCINTs
	*/


#ifndef _DHT_11_H
#define _DHT_11_H

#include<stdint.h>

/*
	 I'm using a custom generated name for the PORTs to avoid 
	 compiler error when passing the name of the PORTs and PINs
	 to the DHT_Config(); function, which takes two parameters;
	     ->>   PORT_NAME and PIN_BIT
	 so one of these three PORT_NAME below shall be used and 
	 passed to the function to configure the PORT to use for
	 the DHT11 sensor data line, and one of the available 
	 PIN_BIT shall be passed to configure the actual pin to 
	 handle the data line of the DHT sensor.
	 */

#define		_PORT_B   	1       //  A custom replacement for [PORTB]
#define 	_PORT_C   	2       //  A custom replacement for [PORTC]
#define 	_PORT_D   	3		//  A custom replacement for [PORTD]


#define 	P0   	0       //  A custom replacement for [PB0] / [PC0] / [PD0]
#define 	P1   	1       //  A custom replacement for [PB1] / [PC1] / [PD1]
#define 	P2   	2       //  A custom replacement for [PB2] / [PC2] / [PD2]
#define 	P3   	3       //  A custom replacement for [PB3] / [PC3] / [PD3]
#define 	P4   	4       //  A custom replacement for [PB4] / [PC4] / [PD4]
#define 	P5   	5       //  A custom replacement for [PB5] / [PC5] / [PD5]
#define 	P6   	6       //  A custom replacement for [PB6] / [PC6] / [PD6]
#define 	P7  	7       //  A custom replacement for [PB7] / [PC7] / [PD7]


#ifndef ERROR_DHT_SETUP
#define ERROR_DHT_SETUP "Error: This MCU is not supported by the 'DHT_11TH.c' library yet!"
#endif


#define 	MCU_BD_LOW     20 /*
			 ~18ms, BD stands for Begin Delay.
			 */
#define 	MCU_BD_HIGH    20 /*
			 ~20us - 40us, if susseccful, the DHT will make the next delay.
			 */
#define 	DHT_READ_DELAY	30 /*
			 This (in ms) is the entire time it takes the DHT to sample
			 all the data beginning from the StartSignal to the end.
			 */
#define 	TIMER_TIMEOUT 	3 	

#define		DHT_HI_LO_THRESHOLD 	\
			((((70 * (F_CPU)) / (8000000UL)) + ((27 * (F_CPU)) / (8000000UL))) / 2) /*
			Formula to calculate the value of DHT_HI_LO_THRESHOLD (number of ticks to
			determine of a bit is '0' or '1').
			*/
#define 	DHT_OVF_THRESHOLD 		\
			((((F_CPU) / 8) / ((UINT16_MAX) - 1)) * TIMER_TIMEOUT)


	

/*
	Takes two parameters for it's argument, the `portValue` could be one of the above
	listed custom replacement for the AVRs generic pin, where;
	~ _PORT_B = 1 and corresponds to the AVR PORTB register.
	~ _PORT_C = 2 and corresponds to the AVR PORTC register.
	~ _PORT_D = 3 and corresponds to the AVR PORTD register.
	While PIN refers to the bit position of the DHT11 data line on the AVR MCU, refer to 
	the top section for more configurations.
	*/
void DHT_Init(uint8_t portValue, uint8_t PIN);

/*
	An API call inluded by a programmer at the top of his `main` source code which is made
	inside an ISR to reduce cost and enhance versatility. It's implimentaiton is 1 of 3;
	you choose the PCINT vect that corresponds to your DHTs pin, like;
				ISR(PCINT0_vect){
					DHT_HandleSignal();
				}
	The above example is for the PCINT available on PORTB. Others include PCINT1_vect for PORTC
	and PCINT2_vect for PORTD.
	*/
void DHT_HandleSignal(void); 

/*
	Returns the current temperature.
	*/
uint8_t DHT_Get_Temp(void);

/*
	Returns the current humidity.
	*/
uint8_t DHT_Get_Humidity(void); 



#endif    /* _DHT_11_H */