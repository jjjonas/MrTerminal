#ifndef _CONFIG_H_
#define _CONFIG_H_

//-------------------------------------------------------------------------------//
//---PIN-DEFINES-- Mr.Termin   --------------------------------------------------//
//-------------------------------------------------------------------------------//

/*			+------ NodeMCU-32-S - Boardlayout  -----------------------------------+ 
 * 
 * 
 *                        +-----------------------------------+
 *                        | [ ] 3V3                   GND [ ] |  	
 *                        | [ ] RST            VSPI-MOSI  [ ] | 23  	
 *                     36 | [ ]                  I2C-SCL  [ ] | 22
 *   Button Up    ->   39 | [ ]                      TX0  [ ] |  1 	
 *   Button Down  ->   34 | [ ]                      RX0  [ ] |  3
 *   Button Left  ->   35 | [ ]                  I2C-SDA  [ ] | 21	Button Right
 *   Button Right ->   32 | [ ]  TOUCH9               GND [ ] |  	  Button GND
 *   Button GND   -|   33 | [ ]  TOUCH8        VSPI-MISO  [ ] | 19  Button Left
 *                     25 | [ ]  DAC1          VSPI-SCK   [ ] | 18 	Button Down
 *   StatusLED    <-   26 | [ ]  DAC2          VSPI-SS    [ ] |  5  Button Up
 *   StatusLED GND-|   27 | [ ]  TOUCH7             TX2   [ ] | 17 <- Modbus DI (grün)
 *                     14 | [ ]  SPI-CLK            RX2   [ ] | 16 -> Modbus RO (gelb)
 *                     12 | [ ]  SPI-MISO         TOUCH0  [ ] |  4  	
 *                        | [ ] GND               TOUCH1  [ ] |  0
 * Modb TX Enab(wh)->  13 | [ ]  SPI-MOSI         TOUCH2  [ ] |  2 -> Board LED
 * Modb RO (ge)   <-    9 | [ ]  RX1       SPI-SS|TOUCH3  [ ] | 15
 *                     10 | [ ]  TX1                      [ ] |  8	
 * (SPI Flash, don't)  11 | [ ]                           [ ] |  7 -| GND (Reset)
 *                        | [ ] VIN                       [ ] |  6 (SPI Flash, don't use) 	
 *                        |        |EN |           |IO0|      |
 *                        |        | * |  |-----|  | * |      |
 *                        +---------------| USB |-------------+  	  	
 *                                        |=====| 
 * 													
*/
#ifdef ESP32

	#define PIN_OUT_ONBOARD_LED 2

    #define PIN_OUT_LED_RED      17  
    #define PIN_OUT_LED_BLUE     18
    #define PIN_OUT_LED_YELLOW   19

	//Control Buttons
	#define PIN_IN_BUTTON_RED     26	
	#define PIN_IN_BUTTON_BLUE    27
	#define PIN_IN_BUTTON_YELLOW  14	

    #define PIN_IN_BUTTON_NEXTGAME 32

#endif

//###############################################################################//
//!---   PIN DEFINITIONs    -----------------------------------------------------//
//###############################################################################//


//############################################################################//
//!---   ALIAS DEFINED    ----------------------------------------------------//
//############################################################################//

#ifdef ESP32
#define BOX_LED_ON 0x1
#define BOX_LED_OFF 0x0
#elif defined(ESP8266)
#define BOX_LED_ON 0x0
#define BOX_LED_OFF 0x1
#endif

/*************************************
* Buttons				             *
*************************************/

#define DEBOUNCE_TIME       20					// [ms] --> not in use
#define DETECT_TIME         50														// [ms] --> not in use

#define LONGPRESS_TIME	    5000				// [ms]	--> not in use


#endif