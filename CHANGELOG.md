# Changelog

All notable changes to the **DHT_11 Library** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)  
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.0] - 2026-04-22

### Added
- Initial release of the DHT_11 high-performance AVR library.
- Interrupt-driven communication using **Pin Change Interrupts (PCINT)**.
- Timer1-based pulse width measurement for accurate bit detection.
- Full implementation of DHT11 communication protocol (40-bit data handling).
- Support for:
  - Humidity (integer + decimal).
  - Temperature (integer + decimal).
- Checksum validation for data integrity.
- Overflow guard mechanism (`timerGuard`) to prevent rapid re-reading.
- Bitwise data assembly using a 32-bit buffer (`temp_nd_hum_data`).
- Separate checksum buffer handling (8-bit).
- Support for multiple AVR ports:
  - `_PORT_B`, `_PORT_C`, `_PORT_D`
- Custom pin abstraction macros (`P0`–`P7`)
- Configurable timing macros:
  - `MCU_BD_LOW`
  - `MCU_BD_HIGH`
  - `DHT_READ_DELAY`
  - `DHT_HI_LO_THRESHOLD`
  - `DHT_OVF_THRESHOLD`
- Internal fail-safe restore mechanism for corrupted reads.
- Low-level register manipulation for maximum performance.
- Example implementation with USART output.
- MIT License.

---

### Technical Details
- Uses **Timer/Counter1** with prescaler of `8`.
- PCINT ISR required for signal edge detection.
- MSB-first bit parsing.
- Dynamic threshold calculation based on `F_CPU`.
- Efficient memory usage (no arrays for bit storage).
- Uses volatile variables for ISR-safe operations.

---

### Known Limitations
- Supports only DHT11 (no DHT22/AM2302 yet).
- Requires manual ISR setup by the user.
- Blocking delay used during read cycle (`_delay_ms`).
- No multi-sensor support.
- No abstraction layer (bare-metal AVR only).

---

## [Unreleased]

### Planned
- Add support for **DHT22 / AM2302**.
- Introduce non-blocking read API.
- Multi-sensor support (multiple pins/devices).
- Optional Arduino-compatible wrapper.
- Improved error reporting (status codes instead of silent fallback).
- Configurable timeout and retry mechanisms.
- Optional floating-point output for decimal precision.

---

## Author

**Dauda Muazu Sulaiman**  
Organization: **KibrisOrder**  
https://ss.kibrisorder.com

---