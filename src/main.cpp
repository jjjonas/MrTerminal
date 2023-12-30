#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>
#include <GameRunner.h>
#include <config.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);
LightMqttSettingsService lightMqttSettingsService =
LightMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &lightMqttSettingsService);

GameRunner Game(PIN_IN_BUTTON_NEXTGAME,
                PIN_OUT_LED_RED, PIN_IN_BUTTON_RED,
                PIN_OUT_LED_BLUE, PIN_IN_BUTTON_BLUE,
                PIN_OUT_LED_YELLOW, PIN_IN_BUTTON_YELLOW);


// GameRunner game(PIN_IN

bool wrapper_Run_GAME1()
{
  return Game.Run_GAME1();
}

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // init the GameRunner

  //TODO: Pass self instance so more elegant, maybe within constructor
  // Game.self = Game;
  Game.Init();




  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightStateService.begin();

  // start the light service
  lightMqttSettingsService.begin();

  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();
}
