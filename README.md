# DHT_11 Library — High-Performance AVR Interface

A **lightweight, high-performance, interrupt-driven DHT11 driver** for AVR microcontrollers.  
This library provides precise timing control using **Pin Change Interrupts (PCINT)** and **Timer1**, enabling accurate and efficient communication with the DHT11 temperature and humidity sensor.

---

## Features

* **Interrupt-driven communication (non-blocking signal capture)**: Uses PCINT to handle timing-critical data pulses, reducing CPU overhead.
* **High timing accuracy (using Timer1)**: Implements a formula-based threshold calculation relative to `F_CPU` for reliable bit detection.
* **Checksum validation (for data integrity)**: Built-in 8-bit checksum verification.
* **Overflow guard mechanism (to prevent premature reads)**: Includes a `timerGuard` mechanism to prevent high-frequency "rapid-fire" polling that can corrupt DHT11 internal logic.
* **Low-level register control (no Arduino abstraction overhead)**: No dependencies on heavy frameworks; talks directly to AVR registers.
* **Portable across modern AVR MCUs** (ATmega328P, ATmega168, ATmega2560, etc.)
* **Efficient bit parsing** (MSB-first data extraction)

---

## Supported Platforms

- AVR microcontrollers with:
  - Pin Change Interrupts (PCINT).
  - Timer/Counter1.

### Examples
- ATmega328P
- ATmega168
- ATmega2560

### Not supported
- Legacy AVRs without PCINT support.

---

## Library Structure

DHT_11/
└──src/
    ├── DHT_11.c        # Core implementation
    ├── DHT_11.h        # Public API
    ├── reg_defs_t.h    # Register abstraction macros (dependency)
    └── USART_D.h       # My custom built USART Library
└── examples/
    └── main.c      # Example usage
└── LICENSE

## File Structure

* `DHT_11.h`: API definitions, threshold formulas, and port mappings.
* `DHT_11.c`: Main implementation logic, including the start signal, parsing, and pulse handling.
* `reg_defs_t.h`: User-defined register abstractions (ensure this is present in your project).

---

## How It Works

### 1. Start Signal
The MCU initiates communication by:
- Pulling the data line LOW for ~18ms.
- Pulling HIGH for ~20–40µs.
- Switching pin to input mode.

### 2. Sensor Response
The DHT11 responds with:
- A LOW pulse (~80µs).
- A HIGH pulse (~80µs).

### 3. Data Transmission (40 bits)
The sensor sends:
- [8-bit Humidity] 
- [8-bit Humidity Decimal]
- [8-bit Temperature]
- [8-bit Temperature Decimal]
- [8-bit Checksum]

### 4. Bit Detection Logic
- HIGH pulse duration determines bit value:
  - Short pulse -> 0.
  - Long pulse -> 1.
- Timer1 is used to measure pulse width.

### 5. Data Parsing
- First 32 bits -> humidity & temperature data.
- Last 8 bits -> checksum.
- Checksum is verified before updating values.

## API Reference

* **`void DHT_Init(uint8_t portValue, uint8_t pinNum)`**

Initializes the DHT11 interface.

Parameters:
- portValue: _PORT_B, _PORT_C, _PORT_D
- pinNum: P0 – P7

* **`void DHT_HandleSignal(void)`**

Handles incoming signal edges.

This must be called inside the correct PCINT ISR.

```c
ISR(PCINT1_vect){
    DHT_HandleSignal();
}
```

* **`uint8_t DHT_Get_Temp(void)`**

Returns the latest temperature reading.

- Automatically triggers a new read if allowed.
- Returns cached value if within guard time.

* **`uint8_t DHT_Get_Humidity(void)`**

Returns the latest humidity reading.

- Same behavior as temperature function.

---

## Example Usage

```c
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "USART_D.h"
#include "DHT_11.h"

ISR(PCINT1_vect){
    DHT_HandleSignal();
}

int main(void){

    DHT_Init(_PORT_C, P0);
    USART_begin();

    USART_print("\r\n==== DHT11_TEST ====\r\n");

    while(1){

        uint8_t temp = DHT_Get_Temp(); 
        uint8_t humd = DHT_Get_Humidity(); 

        USART_print("\r\n");
        USART_print("Current Temperature ->  %d\r\n", temp);
        USART_print("Current Humidity    ->  %d\r\n", humd);
        USART_print("\r\n");

        _delay_ms(2000);
    }
}
```

---

## Configuration Macros

### Port Selection
```c
#define _PORT_B 1
#define _PORT_C 2
#define _PORT_D 3
```

### Pin Selection
```c
#define P0 0
#define P1 1
#define P2 2
#define P3 3
#define P4 4
#define P5 5
#define P6 6
#define P7 7
```

## Timing Configuration

| Macro | Description |
| --- | --- |
| `MCU_BD_LOW` | Start signal LOW duration (`~18ms`) |
| `MCU_BD_HIGH` | Start signal HIGH duration (`~20–40µs`) |
| `DHT_READ_DELAY` | Total read duration (`~30ms`) |
| `DHT_HI_LO_THRESHOLD` | Bit detection threshold |
| `DHT_OVF_THRESHOLD` | Timer overflow guard |

## Timing & Synchronization

- Timer1 Prescaler: 8
- Used to:
  - Measure pulse widths.
  - Detect bit values.
- Overflow interrupt used as:
  - Read guard (~3 seconds) to prevent excessive polling.

## Data Integrity

Checksum formula:
checksum == (humidity + hum_deci + temperature + temp_deci)

If checksum fails:
- Previous valid values are restored.

---

## Important Notes

- Must define the correct PCINT ISR depending on port:
  - PCINT0_vect -> PORTB
  - PCINT1_vect -> PORTC
  - PCINT2_vect -> PORTD

- Ensure:
  - Global interrupts are enabled (`sei()`).
  - CPU frequency (`F_CPU`) is correctly defined.

- Avoid calling read functions too frequently (library enforces guard timing internally).

## Performance Considerations

- Minimal CPU blocking.
- ISR-based signal decoding.
- Efficient bit-shifting (no arrays).
- Direct register manipulation for speed.

## Dependencies

```c
<avr/io.h>
<avr/interrupt.h>
<util/delay.h>
<avr/power.h>
"reg_defs_t.h" // (custom register abstraction)
```

---

## License

This project is licensed under the MIT License.  
See the LICENSE file for details.

## Author

Dauda Muazu Sulaiman  
Organization: KibrisOrder  
https://ss.kibrisorder.com

Special thanks to **KibrisOrder** for supporting the development of high-performance embedded drivers.

## Contributing

Contributions, bug reports, and feature requests are welcome.  
Feel free to fork the project and submit a pull request.

---

## Summary

This library is designed for developers who want:
- Maximum performance
- Precise timing control
- Low-level AVR mastery

If you're working close to the hardware and need reliability, this library is built for that purpose.

---

*"Good and rigid answers are the foundation of stable systems."*
*(I hope this will be useful to the public and the opensource world)*