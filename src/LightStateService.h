#ifndef LightStateService_h
#define LightStateService_h

#include <LightMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define LED_PIN 2

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"

#define DEFAULT_GAME_ID 1
#define DEFAULT_GAME_SPEED 10

// Note that the built-in LED is on when the pin is low on most NodeMCU boards.
// This is because the anode is tied to VCC and the cathode to the GPIO 4 (Arduino pin 2).
#ifdef ESP32
#define LED_ON 0x1
#define LED_OFF 0x0
#elif defined(ESP8266)
#define LED_ON 0x0
#define LED_OFF 0x1
#endif

#define LIGHT_SETTINGS_ENDPOINT_PATH "/rest/lightState"
#define LIGHT_SETTINGS_SOCKET_PATH "/ws/lightState"

class LightState {
 public:
  bool ledOn;
  uint8_t gameId;
  uint8_t gameSpeed;

  static void read(LightState& settings, JsonObject& root) 
  {
    root["led_on"] = settings.ledOn;
    root["game_id"] = settings.gameId;
    root["game_speed"] = settings.gameSpeed;
  }

  static StateUpdateResult update(JsonObject& root, LightState& lightState) 
  {
    bool _oneStateChanged = false;

    boolean newStateLedOn = root["led_on"] | DEFAULT_LED_STATE;
    if (lightState.ledOn != newStateLedOn) 
    {
      lightState.ledOn = newStateLedOn;
      _oneStateChanged = true;
    }

    uint8_t newStateSelectedGameId = root["game_id"] | DEFAULT_GAME_ID;
    if (lightState.gameId != newStateSelectedGameId) 
    {
      lightState.gameId = newStateSelectedGameId;
      _oneStateChanged = true;
    }

    uint8_t newStateSelectedGameSpeed = root["game_speed"] | DEFAULT_GAME_SPEED;
    if (lightState.gameSpeed != newStateSelectedGameSpeed) 
    {
      lightState.gameSpeed = newStateSelectedGameSpeed;
      _oneStateChanged = true;
    }

    return _oneStateChanged ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
  }

  //TODO: remove, its for homa assistant
  static void haRead(LightState& settings, JsonObject& root) {
    root["state"] = settings.ledOn ? ON_STATE : OFF_STATE;
  }

  static StateUpdateResult haUpdate(JsonObject& root, LightState& lightState) {
    String state = root["state"];
    // parse new led state 
    boolean newStateLedOn = false;
    if (state.equals(ON_STATE)) {
      newStateLedOn = true;
    } else if (!state.equals(OFF_STATE)) {
      return StateUpdateResult::ERROR;
    }
    // change the new state, if required
    if (lightState.ledOn != newStateLedOn) {
      lightState.ledOn = newStateLedOn;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class LightStateService : public StatefulService<LightState> {
 public:
  LightStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    LightMqttSettingsService* lightMqttSettingsService);
  void begin();

 private:
  HttpEndpoint<LightState> _httpEndpoint;
  MqttPubSub<LightState> _mqttPubSub;
  WebSocketTxRx<LightState> _webSocket;
  AsyncMqttClient* _mqttClient;
  LightMqttSettingsService* _lightMqttSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
