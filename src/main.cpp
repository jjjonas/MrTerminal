#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>


#include "pins.h"
#include "OneButton.h"
#include "Blinker.h"
#include "CurtainStepper.h"

#define SERIAL_BAUD_RATE 115200



#define CLICK_TICKS                     80   // [ms]
#define DEBOUNCE_TIME                   25    // [ms]
#define PRESS_DETECT_TIME               500   // [ms]

AsyncWebServer server(80);
ESP8266React esp8266React(&server);
LightMqttSettingsService lightMqttSettingsService =
    LightMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &lightMqttSettingsService);



A4988 stepper(MOTOR_STEPS, PIN_OUT_DIRECTION, PIN_OUT_STEP, PIN_OUT_SLEEP);
Blinker statusBlinker(PIN_OUT_ONBOARD_LED);

OneButton buttonStart(PIN_IN_BUTTON, true, true);

CurtainStepper curtain = CurtainStepper(stepper, statusBlinker, PIN_IN_ENDSTOP);

void wrapToogleOpenCurtain() { curtain.toogleOpenCurtain(); }
void wrapStartHomeingToEndstop() { curtain.startHomeingToEndstop(); }
void wrapStartCloseing() { curtain.startClosing(); }
void wrapStopClosing() { curtain.stopClosing(); }

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  // ENDSTOP

  // pinMode(PIN_GND_ENDSTOP, OUTPUT);
  // digitalWrite(PIN_GND_ENDSTOP, LOW);

  // BUTTON
  pinMode(PIN_IN_BUTTON, INPUT_PULLUP);
  // pinMode(PIN_GND_BUTTON, OUTPUT);
  // digitalWrite(PIN_GND_BUTTON, LOW);


	buttonStart.setClickTicks(CLICK_TICKS); 
  buttonStart.setDebounceTicks(DEBOUNCE_TIME);
	buttonStart.attachClick(wrapToogleOpenCurtain);
  
  buttonStart.attachDoubleClick(wrapStartHomeingToEndstop);
  
  buttonStart.setPressTicks(PRESS_DETECT_TIME);
  buttonStart.attachLongPressStart(wrapStartCloseing);
  buttonStart.attachLongPressStop(wrapStopClosing);

  curtain.begin();


  // load the initial light settings
  lightStateService.begin();

  // start the light service
  lightMqttSettingsService.begin();

  // start the server
  server.begin();
}

void loop() {
  
  
  buttonStart.tick();
  
  curtain.tick();

  // run the framework's loop function
  esp8266React.loop();


}
