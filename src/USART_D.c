/*
 * USART_D Library - High-performance USART communication
 * * Copyright (c) 2026 Dauda Muazu Sulaiman
 * * Copyright (c) 2026 KibrisOrder                <https://ss.kibrisorder.com>
 * * Author: Dauda Muazu Sulaiman
 * * Organization: KibrisOrder
 * * All rights reserved.
 * * This software is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */



#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/setbaud.h>
#include<util/delay.h>
#include <stdarg.h>

#include "USART_D.h"
#include "reg_defs_t.h"

#define TX_BUFFER 32
#define RX_BUFFER 64


volatile char tx_buf[TX_BUFFER];
volatile static char rx_buf[RX_BUFFER];

volatile uint8_t txReadPtr = 0;
volatile uint8_t txWritePtr = 0;

volatile uint8_t rxReadPtr;
volatile uint8_t rx_end_flag;
volatile static uint8_t timeout_count;
 

void USART_begin(void);
char USART_receive(void);
void USART_flush(void);
static void printNumber(int16_t num);
static inline void transferByte(char buffer);
void USART_print(const char buffer[], ...);

 
ISR(_UDR_EMPTY_){
	if(txWritePtr != txReadPtr){

		_DATA_SFR_ = tx_buf[txReadPtr];

		txReadPtr = (txReadPtr + 1) & (TX_BUFFER - 1);

	}else{

		_UCSR_2_SFR &= ~(1 << _UDRI_);
	} 
}
 
ISR(_RX_COMPLETE_){

	if(!rx_end_flag){

		char data = _DATA_SFR_;

		if(data == '\r'){

			rx_end_flag = 1; 
		}

		else if(rxReadPtr >= (RX_BUFFER - 1)){
			
			rx_end_flag = 1;
		}

		else if (data == '\b' || data == 127) { // Backspace or Delete
		    
		    if (rxReadPtr > 0) {

		        rxReadPtr--;
		    }
		}
 	
 		else{
 			
 			rx_buf[rxReadPtr] = data;
			rxReadPtr = (rxReadPtr + 1) & (RX_BUFFER - 1); 
		}

		timeout_count = 0; 
	} 
}

 
ISR(_TIMER2_COMPA_){

	if(!rx_end_flag){

		timeout_count++;
	
		if(timeout_count >= 150){

			rx_end_flag = 1;
		} 
	}
}

 
static inline void initTIMER_2(void) {

    _OCR2A_ = _OCR_VAL_;

	_TCR2A_ |= (1 << _WGM21_);
	_TCR2B_ |= (1 << _CS22_) | (1 << _CS21_) | (1 << _CS20_);

	_TIMSK2_ |= (1 << _OCIE2A_);
}

 
void USART_begin(void){

	_UBRR_HIGH_ = UBRRH_VALUE;
	_UBRR_LOW_ = UBRRL_VALUE;

	#if USE_2X
	_UCSR_1_SFR |= (1 << _U2X_);
	#else
	_UCSR_1_SFR &= ~(1 << _U2X_);
	#endif

	_UCSR_2_SFR |= (1 << _TXE_) | (1 << _RXE_);
	_UCSR_3_SFR |= (1 << _UCSZ1_) | (1 << _UCSZ0_);
	
	sei();
}
 

char USART_getByte(void){

	loop_until_bit_is_set(_UCSR_1_SFR, _RXC_);

	char receivedByte = _DATA_SFR_;

	transferByte(receivedByte);

	return receivedByte;
}

 
const char* USART_getString(void){ 

	USART_flush();

	rx_end_flag = 0;
	rxReadPtr = 0;
	rx_buf[0] = '\0'; 
	timeout_count = 0;
	TCNT2 = 0;

	_UCSR_2_SFR |= (1 << _RXCI_);
	initTIMER_2();

	while(!rx_end_flag){};

	_UCSR_2_SFR &= ~(1 << _RXCI_);
	_TCR2B_ = 0;
	rx_buf[rxReadPtr] = '\0';

	//debug output
	// USART_print("INPUT FROM HEADER = %s.\r\n", rx_buf);
	return (const char*)rx_buf;
}

 
void USART_flush(void){

	unsigned char dummy;

	while(bit_is_set(_UCSR_1_SFR, _RXC_)){
		dummy = _DATA_SFR_;
	}
}

 
static inline void transferByte(char buffer){

	uint8_t nextPtr = (txWritePtr + 1) & (TX_BUFFER - 1);
    
    while(nextPtr == txReadPtr){};

    tx_buf[txWritePtr] = buffer;
    txWritePtr = nextPtr;

	_UCSR_2_SFR |= (1 << _UDRI_);
}
 

static void printNumber(int16_t num){

	char buf[6];
	int8_t i = 0;

	if(num < 0){
		num *= -1;
		transferByte('-');
	}

	do{
		buf[i++] = (num % 10) + '0';
		num /= 10;
	}while(num > 0);

	while(i){
		transferByte(buf[--i]);
	}
}

 
void USART_print(const char buffer[], ...){

	if(!bit_is_set(SREG, 7)){
		sei();
	}

	va_list bytes;
	va_start(bytes, buffer);

	//char *i;

	for(uint8_t i = 0; buffer[i] != '\0'; i++){

		if(buffer[i] != '%'){
			transferByte(buffer[i]);

			continue;
		}

		i++;  // *i == '%', checking the next character

		switch(buffer[i]){
			case 'd': {//an integer is encountered
				int16_t number = va_arg(bytes, int);
	
				printNumber(number);
				break;
			}
			case 'c': {//a character is encountered
				char character = (char)va_arg(bytes, int);
	
				transferByte(character);
				break;
			}
			case 's': {//a string is encountered
				char *string = va_arg(bytes, char*);
	
				while(*string){
					transferByte(*string++);
				}
				break;
			}
			case '%': {//a literal '%' character is encountered
				transferByte('%');
				break;
			}
			default: {//specifier can't be identified
				transferByte(buffer[i]);
				break;
			}
		}
	}

	va_end(bytes);
} 