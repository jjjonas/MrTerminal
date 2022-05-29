#ifndef _pins_H_
#define _pins_H_

//############################################################################//
//!---   PIN DEFINITIONs    --------------------------------------------------//
//############################################################################//

#ifdef ESP8266
/******************
 * ESP8266    *
 ******************/

/*  +------ Wemos - D1 mini ----------------------------------+
 *  |------ |-------------------------------|---------------|--------------------
 *  | Pin	  | Function									  	| ESP-8266 Pin	| Usage
 *  |------ |-------------------------------|---------------|--------------------
 *  | TX		| TXD														| TXD						|
 *  | RX		| RXD														| RXD						|
 *  | A0		| Analog input, max 3.3V input	| A0						| 
 *  | D0		| IO														| GPIO16				| 
 *  | D1		| IO, SCL												| GPIO5					| 
 *  | D2		| IO, SDA												| GPIO4					| 
 *  | D3		| IO, 10k Pull-up								| GPIO0					| 
 *  | D4		| IO, 10k Pull-up, BUILTIN_LED	| GPIO2					| Board LED
 *  | D5		| IO, SCK												| GPIO14				| 
 *  | D6		| IO, MISO											| GPIO12				| 
 *  | D7		| IO, MOSI											| GPIO13				| 
 *  | D8		| IO, 10k Pull-down, SS					| GPIO15				| 
 *  | G		  | Ground												| GND						|
 *  | 5V		| 5V														| -			-				|
 *  | 3V3	  | 3.3V													| 3.3V					|
 *  | RST	  | Reset													| RST						|
 *  |-------|-------------------------------|---------------|--------------------
 */

// LED-Pins
#define PIN_OUT_ONBOARD_LED LED_BUILTIN //D4


// Button/Switch Pins

#define PIN_IN_BUTTON D1
// #define PIN_GND_BUTTON D2

#define PIN_IN_ENDSTOP D2
// #define PIN_GND_ENDSTOP D5

// Steoper Output

#define MOTOR_STEPS  200
#define MICROSTEPS 16

#define PIN_OUT_DIRECTION D6
#define PIN_OUT_STEP D7
#define PIN_OUT_SLEEP D5 // optional (just delete SLEEP from everywhere if not used)

/*
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */
#endif

#endif /* _pins_H_ */