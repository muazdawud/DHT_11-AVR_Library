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




#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<avr/power.h>


#include "DHT_11.h"
#include "reg_defs_t.h"
 

/*
	The DHT_Device struct that is used to store and reference
	the PORT, PIN, DDR and the bit position of the corresponding
	configured DHT11 data line.
	*/
typedef struct {
    volatile uint8_t* port;
    volatile uint8_t* ddr;
    volatile uint8_t* pin;
    uint8_t bit;
} DHT_Device;

DHT_Device mySensor;


/*
	~ temp_nd_hum_data -; is a 32-bit int that stores the entire DHT11
	data bit sequence(excluding the checksum).

	~ checksum -; is the 8-bit int that stores the LSB of the DHT11 data
	sequence, which is used for proof checking.

	~ temperature -; stores the temperature.

	~ humidity -; stores the humidity.

	~ temp_deci -; stores the decimals of the temperature.

	~ hum_deci -; stores the decimals of the humidity.
	*/
volatile static uint32_t temp_nd_hum_data = 0;
volatile static uint8_t checksum = 0;
volatile static uint8_t temperature = 0;
volatile static uint8_t humidity = 0;
volatile static uint8_t temp_deci = 0;
volatile static uint8_t hum_deci = 0;

/*	
	All this entire temp-suffixed variables are temporary value holders
	that serve as restore points (in any uncertain issues).
	*/
volatile static uint8_t temp_t = 0;
volatile static uint8_t temp_h = 0;
// volatile static uint8_t temp_t_d = 0;
// volatile static uint8_t temp_h_d = 0;
uint8_t temp_SREG;


/*
	~ timerGuard -; is a timer overflow guard that triggers at the end of any
	data collection (either temperature or humidity) which last for ~3seconds 
	to avoid unwanted triggers/fast-data fetch by programs, to avoid data 
	corruption from the DHT11.

	~ timestamp -; is the variable that is used to analyse the number of ticks
	to determine if a bit is `1` or `0`.

	~ pcint_cycle -; is a guard to escape/ignore three sequences from the DHT11
	which are; - The first two response signals from the DHT11 which last for ~40us,
			and - The first data available signal from the DHT11 which last for
				  about 50us.

	~ bit_position_tnhd -; is used to track the exact position of the data/bit in
	our `temp_nd_hum_data` variable (which stores the entire 32 data bits).
	*/
volatile static uint8_t timerGuard = 0;
volatile static uint16_t timestamp = 0;
volatile static uint8_t pcint_cycle = 0;
volatile static uint8_t bit_position_tnhd = 0; 
 


/*
	Library implementation-only functions;
	*/
static void DHT_StartSignal(void);
static void DHT_Parse_Data(void); 
static inline uint8_t DHT_Verify_Checksum(void);
static inline void initTIMER_1(void);
static inline void DHT_end(void);


/*
	Available API calls for running and interfacing DHT11
	**/
void DHT_Init(uint8_t portValue, uint8_t PIN);
void DHT_HandleSignal(void);
uint8_t DHT_Get_Temp(void);
uint8_t DHT_Get_Humidity(void); 



ISR(_TIMER1_OVF_){

	timerGuard++;

	if(timerGuard >= (DHT_OVF_THRESHOLD)){//make a formula to calculate 90 instead of writing it
		timerGuard = 0;
		_TCR1B_ = 0;
	}
}



/*
	The DHT_StartSignal(void) function <which is a local function> starts by saving the SREG
	into a temporary SREG restore point variable and then disables global interrupts(to avoid)
	other unknown interrupt routines), resets all our variables to new and zero values, then
	starts sending out communication start signals to the DHT11s bit position in it's pre-defined
	PORT, by sending a LOW signal for anything above 18ms, then a HIGH signal for about 20-40us,
	then releasing the line and setting as input to enable PCINTand to get DHT11s response, then
	sets the TIMER/COUNTER1 with a pre-scaler of 8, and lastly enable global ISRs.
	*/
static void DHT_StartSignal(void) {

	temp_SREG = SREG;
	cli();

	TCNT1 = 0;

	temp_t = temperature;
	// temp_t_d = temp_deci;
	temp_h = humidity;
	// temp_h_d = hum_deci;

	timestamp = 0;
	pcint_cycle = 0;
	timerGuard = 0;
	bit_position_tnhd = 0;
	temp_nd_hum_data = 0;
	checksum = 0;

    *(mySensor.ddr) |= (1 << mySensor.bit);
    *(mySensor.port) &= ~(1 << mySensor.bit);
    _delay_ms(MCU_BD_LOW);

    *(mySensor.port) |= (1 << mySensor.bit);
    _delay_us(MCU_BD_HIGH);
    *(mySensor.ddr) &= ~(1 << mySensor.bit);

    _TCR1B_ |= (1 << _CS11_);
    
    sei();
}


/*
	Extracts the data from our Master `temp_nd_hum_data` variable which stores all
	our bit sequences in MSBFIRST format, where;
	humidity = MSB sequence.
	temperature = MSB sequence right after the hum_deci sequence (16-positoins after the first MSB).
	and the rest, makes the checksum check to verify data integrity and set the new values of humidity
	and temperature.
	*/
static void DHT_Parse_Data(void){

	cli();

	if(temp_nd_hum_data){

		humidity = (temp_nd_hum_data >> 24);

		hum_deci = (temp_nd_hum_data >> 16);

		temperature = (temp_nd_hum_data >> 8);

		temp_deci = (temp_nd_hum_data);

		if(!DHT_Verify_Checksum()){
			temperature = temp_t;
			// temp_deci = temp_t_d;
			humidity = temp_h;
			// hum_deci = temp_h_d;
		}
	}
}


static inline uint8_t DHT_Verify_Checksum(void){

	uint8_t sum = humidity + hum_deci + temperature + temp_deci;

	if(checksum == sum){
		return 1;
	}

	return 0;
}


/* Ensure NormalMode for TIMER/COUNTER1 is enabled */
static inline void initTIMER_1(void){

	_TCR1B_ &= ~(1 << _WGM12_) & ~(1 << _WGM13_);
	_TCR1A_ &= ~(1 << _WGM10_) & ~(1 << _WGM11_);
	
}


/* 
	Restore the SREG values which where stored in the DHT_StartSignal() function
	and enable TIMER/COUNTER1 to make the timerGuard check.
*/
static inline void DHT_end(void){
	DHT_Parse_Data(); 

	SREG = temp_SREG;
	_TIMSK1_ |= (1 << _TOIE1_);
}




void DHT_Init(uint8_t portSelection, uint8_t pinNum) {

	clock_prescale_set(clock_div_1);

    mySensor.bit = pinNum;
    
    switch(portSelection) {
        case 1:
            mySensor.port = &PORTB;
            mySensor.ddr  = &DDRB;
            mySensor.pin  = &PINB;

            #if defined(PCICR)
	            PCICR |= (1 << PCIE0);
	            PCMSK0 |= (1 << pinNum);
            #else 
            	#error ERROR_DHT_SETUP
            #endif

            break;
        case 2:
            mySensor.port = &PORTC;
            mySensor.ddr  = &DDRC;
            mySensor.pin  = &PINC;

            #if defined(PCICR)
	            PCICR |= (1 << PCIE1);
	            PCMSK1 |= (1 << pinNum);
            #else 
            	#error ERROR_DHT_SETUP
            #endif

            break;
        case 3:
            mySensor.port = &PORTD;
            mySensor.ddr  = &DDRD;
            mySensor.pin  = &PIND;

            #if defined(PCICR)
	            PCICR |= (1 << PCIE2);
	            PCMSK2 |= (1 << pinNum);
            #else 
            	#error ERROR_DHT_SETUP
            #endif

            break;
    }

    initTIMER_1();
}


/*
	If bit is HIGH signaling there is a bit/Data Output on the line, timestamp the TCNT1 register
	to calculate the elapsed time when the bit is HIGH before getting a new Data Output Coming Signal
	(which is a LOW signal), this helps to calculate and decide if a bit is `1` or `0`.
	*/ 
void DHT_HandleSignal(void){ 

	if(pcint_cycle > 2){

		if(bit_is_set(*(mySensor.pin), mySensor.bit)){

			timestamp = TCNT1;
		}
		else{ 

			uint8_t bit_ = ((TCNT1 - timestamp) >  (DHT_HI_LO_THRESHOLD)) ? 1 : 0;
		 
			if(bit_position_tnhd < 32){

					temp_nd_hum_data = (temp_nd_hum_data << 1) | bit_;
			}

			else if(bit_position_tnhd < 40){

					checksum = (checksum << 1) | bit_;
			}

			bit_position_tnhd++;
		}
	}

	pcint_cycle++;
}


uint8_t DHT_Get_Temp(void){

	if(timerGuard){
		return temperature;
	}

	DHT_StartSignal();
	_delay_ms(DHT_READ_DELAY - MCU_BD_LOW);
	DHT_end();

	return temperature;
}


uint8_t DHT_Get_Humidity(void){

	if(timerGuard){
		return humidity;
	}

	DHT_StartSignal();
	_delay_ms(DHT_READ_DELAY - MCU_BD_LOW);
	DHT_end();

	return humidity;
}