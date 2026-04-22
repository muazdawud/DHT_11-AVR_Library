/*
 * REG_DEFS_T.H Implementation - Master File that made REG_DEFS_T possible
 * * Copyright (c) 2026 Dauda Muazu Sulaiman
 * * Copyright (c) 2026 KibrisOrder                <https://ss.kibrisorder.com>
 * * Author: Dauda Muazu Sulaiman
 * * Organization: KibrisOrder
 * * All rights reserved.
 * * This software is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */


#include<bits/stdc++.h>

using namespace std;

string arr_1[] = {
	"TCCR0A", "TCCR0B", "OCR0A", "OCR0B", "TIMSK0", "TIMER0_COMPA_vect", "TIMER0_COMPB_vect", "TIMER0_OVF_vect",
	"TCCR1A", "TCCR1B", "TCCR1C", "OCR1A", "OCR1B", "TIMSK1", "TIMER1_CAPT_vect", "TIMER1_COMPA_vect", "TIMER1_COMPB_vect", "TIMER1_OVF_vect",
	"TCCR2A", "TCCR2B", "OCR2A", "OCR2B", "TIMSK2", "TIMER2_COMPA_vect", "TIMER2_COMPB_vect", "TIMER2_OVF_vect",
	"UDR0", "UCSR0A", "UCSR0B", "UCSR0C", "UBRR0", "USART_RX_vect", "USART_UDRE_vect", "USART_TX_vect"
};

string arr_2[] = { 
	"TCCR0", "OCR0", "TIMSK", "TIMER0_COMP_vect", "TIMER0_OVF_vect",
	"TCCR1A", "TCCR1B", "OCR1A", "OCR1B", "TIMSK", "TIMER1_CAPT_vect", "TIMER1_COMPA_vect", "TIMER1_COMPB_vect", "TIMER1_OVF_vect",
	"TCCR2", "OCR2", "TIMSK", "TIMER2_COMP_vect", "TIMER2_OVF_vect",
	"UDR", "UCSRA", "UCSRB", "URSEL", "USART_RXC_vect", "USART_UDRE_vect", "USART_TXC_vect"
};

int main(void){

	string temp_1;
	string temp_2;

	fstream file;

	file.open("reg_defs_t.h", ios::out);

	if(!file) return 1;

	for(int i = 0; i < 5; i++){
		file << endl;
	}

	file << "#ifndef _REG_DEFS_T_H" << endl;
	file << "#define _REG_DEFS_T_H" << endl;

	for(int i = 0; i < 5; i++){
		file << " " << endl;
	}

	for(int i = 0; i < 50; i++){

		if(i < (int)sizeof(arr_1)){
			temp_1 = arr_1[i];
		}

		if(i < (int)sizeof(arr_2)){
			temp_2 = arr_2[i];
		}
			

		file << " " << endl;
		file << " " << endl;
		file << "/*  */" << endl;
		file << "#if defined(" << temp_1 << ")" << endl;

		for(int j = 0; j < 8; j++){
			file << "	#define" << endl;

		}

		file << "#elif defined(" << temp_2 << ")" << endl;

		for(int j = 0; j < 8; j++){
			file << "	#define" << endl;

		}

		file << "#endif" << endl;
	}

	for(int i = 0; i < 3; i++){
		file << " " << endl;
	}

	file << "#endif /* _REG_DEFS_T_H */" << endl;

	return 0;
}