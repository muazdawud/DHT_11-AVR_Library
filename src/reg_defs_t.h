/*
 * REG_DEFS_T.H Header - Cross Platform Integration Layer
 * * Copyright (c) 2026 Dauda Muazu Sulaiman
 * * Copyright (c) 2026 KibrisOrder                <https://ss.kibrisorder.com>
 * * Author: Dauda Muazu Sulaiman
 * * Organization: KibrisOrder
 * * All rights reserved.
 * * This software is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */




/*
	GENERIC REG_DEFS_T.H header -; ALLOWING FOR CROSS_PLATFORM INTEGRATION
	*/

#ifndef _AVR_IO_H_
	#	error "Error: <avr/io.h> must be included before 'reg_defs.h'."
#endif



#ifndef _REG_DEFS_T_H
#define _REG_DEFS_T_H  1
 
 

/*
	First (#if) is for MODERN AVR (ATmega328P, 168, 2560,  etc.)

	Second (#if) is for LEGACY AVR (ATmega8, 16, 32,  etc.)
*/


#ifndef _ERROR_D_
	#define _ERROR_D_ "Error: This MCU is not supported by the 'reg_defs.h' header yet!"
#endif

/* Output Compare Register Value */
#ifndef _OCR_VAL_
	#define _OCR_VAL_ ((F_CPU / 1024) * 16 / 1000 - 1)
#endif

 
 
/* Timer/Counter0 Control Register (A) */
#if defined(TCCR0A)
	#define 	_TCR0A_		TCCR0A
	#define 	_WGM00_		WGM00
	#define 	_WGM01_		WGM01
#elif defined(TCCR0)
	#define 	_TCR0A_		TCCR0
	#define 	_WGM00_		WGM00
	#define 	_WGM01_		WGM01
#else
    #error _ERROR_D_
#endif

 
 
/* Timer/Counter0 Control Register (B) */
#if defined(TCCR0B)
	#define 	_TCR0B_		TCCR0B
	#define 	_WGM02_		WGM02
	#define 	_CS00_		CS00
	#define 	_CS01_		CS01
	#define 	_CS02_		CS02
#elif defined(TCCR0)
	#define 	_TCR0B_		TCCR0
	#define 	_CS00_		CS00
	#define 	_CS01_		CS01
	#define 	_CS02_		CS02
#else
    #error _ERROR_D_
#endif
 
 
/* Output Compare Register 0 (A) */
#if defined(OCR0A)
	#define 	_OCR0A_		OCR0A
#elif defined(OCR0)
	#define 	_OCR0A_ 	OCR0
#else
    #error _ERROR_D_
#endif
 
 
/* Output Compare Register 0 (B) */
#if defined(OCR0B)
	#define 	_OCR0B_		OCR0B
#elif defined(OCR0)
	#define 	_OCR0B_ 	OCR0
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter0 Interrupt Mask Register */
#if defined(TIMSK0)
	#define 	_TIMSK0_ 	TIMSK0
	#define 	_TOIE0_ 	TOIE0
	#define 	_OCIE0A_	OCIE0A
	#define 	_OCIE0B_ 	OCIE0B
#elif defined(TIMSK)
	#define 	_TIMSK0_ 	TIMSK
	#define 	_TOIE0_ 	TOIE0
	#define 	_OCIE0A_ 	OCIE0
	#define 	_OCIE0B_ 	OCIE0
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter0 Interrupt Vectors */
#if defined(TIMER0_COMPA_vect)
	#define 	_TIMER0_COMPA_		TIMER0_COMPA_vect
	#define 	_TIMER0_COMPB_		TIMER0_COMPB_vect
	#define 	_TIMER0_OVF_		TIMER0_OVF_vect
#elif defined(TIMER0_COMP_vect)
	#define 	_TIMER0_COMPA_		TIMER0_COMP_vect
	#define 	_TIMER0_COMPB_		TIMER0_COMP_vect
	#define 	_TIMER0_OVF_ 		TIMER0_OVF_vect
#else
    #error _ERROR_D_
#endif
  
/*  Timer/Counter1 Control Register (A) */
#if defined(TCCR1A)
	#define 	_TCR1A_		TCCR1A
	#define 	_WGM10_		WGM10
	#define 	_WGM11_ 	WGM11
#elif defined(TCCR1A)
	#define 	_TCR1A_		TCCR1A
	#define 	_WGM10_		WGM10
	#define 	_WGM11_ 	WGM11
#else
    #error _ERROR_D_
#endif 
 
/*  Timer/Counter1 Control Register (B) */
#if defined(TCCR1B)
	#define 	_TCR1B_		TCCR1B
	#define 	_CS10_		CS10
	#define 	_CS11_		CS11
	#define 	_CS12_		CS12
	#define 	_WGM12_		WGM12
	#define 	_WGM13_  	WGM13
#elif defined(TCCR1B)
	#define 	_TCR1B_		TCCR1B
	#define 	_CS10_		CS10
	#define 	_CS11_		CS11
	#define 	_CS12_		CS12
	#define 	_WGM12_		WGM12
	#define 	_WGM13_  	WGM13
#else
    #error _ERROR_D_
#endif
  
 
/*  Output Compare Register 1 (A) */
#if defined(OCR1A)
	#define 	_OCR1A_		OCR1A
	#define 	_OCR1AL_ 	OCR1AL
	#define 	_OCR1AH_ 	OCR1AH
#elif defined(OCR1A)
	#define 	_OCR1A_		OCR1A
	#define 	_OCR1AL_ 	OCR1AL
	#define 	_OCR1AH_ 	OCR1AH
#else
    #error _ERROR_D_
#endif
 
 
/*  Output Compare Register 1 (B) */
#if defined(OCR1B)
	#define 	_OCR1B_		OCR1B
	#define 	_OCR1BL_ 	OCR1BL
	#define 	_OCR1BH_ 	OCR1BH
#elif defined(OCR1B)
	#define 	_OCR1B_		OCR1B
	#define 	_OCR1BL_ 	OCR1BL
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter1 Interrupt Mask Register */
#if defined(TIMSK1)
	#define 	_TIMSK1_ 	TIMSK1
	#define 	_TOIE1_ 	TOIE1
	#define 	_OCIE1A_	OCIE1A
	#define 	_OCIE1B_ 	OCIE1B
#elif defined(TIMSK)
	#define 	_TIMSK1_ 	TIMSK
	#define 	_TOIE1_ 	TOIE1
	#define 	_OCIE1A_	OCIE1A
	#define 	_OCIE1B_ 	OCIE1B
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter1 Interrupt Vectors */
#if defined(TIMER1_COMPA_vect)
	#define 	_TIMER1_COMPA_		TIMER1_COMPA_vect
	#define 	_TIMER1_COMPB_ 		TIMER1_COMPB_vect
	#define 	_TIMER1_OVF_ 		TIMER1_OVF_vect
#elif defined(TIMER1_COMPA_vect)
	#define 	_TIMER1_COMPA_		TIMER1_COMPA_vect
	#define 	_TIMER1_COMPB_ 		TIMER1_COMPB_vect
	#define 	_TIMER1_OVF_ 		TIMER1_OVF_vect
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter2 Control Register (A) */
#if defined(TCCR2A)
	#define 	_TCR2A_		TCCR2A
	#define 	_WGM20_		WGM20
	#define 	_WGM21_ 	WGM21
#elif defined(TCCR2)
	#define 	_TCR2A_   	TCCR2
	#define 	_WGM20_		WGM20
	#define 	_WGM21_		WGM21
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter2 Control Register (B) */
#if defined(TCCR2B)
	#define 	_TCR2B_ 	TCCR2B
	#define 	_CS20_		CS20
	#define 	_CS21_		CS21
	#define 	_CS22_		CS22
	#define 	_WGM22_		WGM22
#elif defined(TCCR2)
	#define 	_TCR2B_ 	TCCR2
	#define 	_CS20_		CS20
	#define 	_CS21_		CS21
	#define 	_CS22_		CS22
#else
    #error _ERROR_D_
#endif
 
 
/* Output Compare Register 2 (A) */
#if defined(OCR2A)
	#define 	_OCR2A_		OCR2A
#elif defined(OCR2)
	#define 	_OCR2A_		OCR2
#else
    #error _ERROR_D_
#endif
 
 
/* Output Compare Register 2 (B) */
#if defined(OCR2B)
	#define 	_OCR2B_ 	OCR2B
#elif defined(OCR2)
	#define 	_OCR2B_		OCR2
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter2 Interrupt Mask Register */
#if defined(TIMSK2)
	#define 	_TIMSK2_ 	TIMSK2
	#define 	_TOIE2_ 	TOIE2
	#define 	_OCIE2A_	OCIE2A
	#define 	_OCIE2B_ 	OCIE2B
#elif defined(TIMSK)
	#define 	_TIMSK2_ 	TIMSK
	#define 	_TOIE2_ 	TOIE2
	#define 	_OCIE2A_	OCIE2
	#define 	_OCIE2B_ 	OCIE2
#else
    #error _ERROR_D_
#endif
 
 
/* Timer/Counter2 Interrupt Vectors */
#if defined(TIMER2_COMPA_vect)
	#define 	_TIMER2_COMPA_ 		TIMER2_COMPA_vect
	#define 	_TIMER2_COMPB_		TIMER2_COMPB_vect
	#define 	_TIMER2_OVF_ 		TIMER2_OVF_vect
#elif defined(TIMER2_COMP_vect)
	#define 	_TIMER2_COMPA_ 		TIMER2_COMP_vect
	#define 	_TIMER2_COMPB_		TIMER2_COMP_vect
	#define 	_TIMER2_OVF_ 		TIMER2_OVF_vect
#else
    #error _ERROR_D_
#endif
 
 
/* USART I/O Data Register n */
#if defined(UDR0)
	#define 	_DATA_SFR_ 		UDR0
#elif defined(UDR)
	#define 	_DATA_SFR_		UDR
#else
    #error _ERROR_D_
#endif
 
 
/* USART Control and Status Register n (A) */
#if defined(UCSR0A)
	#define 	_UCSR_1_SFR 	UCSR0A
	#define 	_U2X_ 			U2X0
	#define 	_RXC_ 			RXC0
	#define 	_TXC_ 			TXC0
#elif defined(UCSRA)
	#define 	_UCSR_1_SFR 	UCSRA
	#define 	_U2X_ 			U2X
	#define 	_RXC_ 			RXC
	#define 	_TXC_ 			TXC
#else
    #error _ERROR_D_
#endif


/* USART Control and Status Register n (A) */
#if defined(UCSR0B)
	#define 	_UCSR_2_SFR 	UCSR0B
	#define 	_TXE_ 			TXEN0
	#define 	_RXE_ 			RXEN0
	#define 	_RXCI_ 			RXCIE0
	#define 	_TXCI_ 			TXCIE0
	#define 	_UDRI_ 			UDRIE0
	#define 	_UCSZ2_ 		UCSZ02
#elif defined(UCSRB)
	#define 	_UCSR_2_SFR 	UCSR0B
	#define 	_TXE_ 			TXEN
	#define 	_RXE_ 			RXEN
	#define 	_RXCI_ 			RXCIE
	#define 	_TXCI_ 			TXCIE
	#define 	_UDRI_ 			UDRIE
	#define 	_UCSZ2_ 		UCSZ2
#else
    #error _ERROR_D_
#endif


/* USART Control and Status Register n (A) */
#if defined(UCSR0C)
	#define 	_UCSR_3_SFR 	UCSR0C
	#define  	_UCSZ0_ 		UCSZ00
	#define  	_UCSZ1_ 		UCSZ01
#elif defined(UCSRC)
	#define 	_UCSR_3_SFR 	UCSRC
	#define  	_UCSZ0_ 		UCSZ0
	#define  	_UCSZ1_ 		UCSZ1
	#define 	_URSEL_   		URSEL
#else
    #error _ERROR_D_
#endif


/* USART Baud Rate Registers */
#if defined(UBRR0H)
	#define 	_UBRR_HIGH_ 	UBRR0H
	#define 	_UBRR_LOW_ 		UBRR0L
#elif defined(UBRRH)
	#define 	_UBRR_HIGH_ 	UBRRH
	#define 	_UBRR_LOW_ 		UBRRL
#else
    #error _ERROR_D_
#endif


/* USART Interrupt Vectors */
#if defined(USART_RX_vect)
	#define 	_RX_COMPLETE_ 		USART_RX_vect
	#define 	_TX_COMPLETE_  		USART_TX_vect
	#define 	_UDR_EMPTY_			USART_UDRE_vect
#elif defined(USART_RXC_vect)
	#define 	_RX_COMPLETE_ 		USART_RXC_vect
	#define 	_TX_COMPLETE_ 		USART_TXC_vect
	#define 	_UDR_EMPTY_ 		USART_UDRE_vect
#else
    #error _ERROR_D_
#endif


#endif /* _REG_DEFS_T_H */