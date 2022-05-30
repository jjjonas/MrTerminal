#ifndef CurtainStateService_h
#define CurtainStateService_h

#include <CurtainMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define LED_PIN 2

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"

// Note that the built-in LED is on when the pin is low on most NodeMCU boards.
// This is because the anode is tied to VCC and the cathode to the GPIO 4 (Arduino pin 2).
#define LED_ON 0x0
#define LED_OFF 0x1

#define CURTAIN_SETTINGS_ENDPOINT_PATH "/rest/curtainState"
#define CURTAIN_SETTINGS_SOCKET_PATH "/ws/curtainState"

class CurtainState {
 public:
  bool ledOn;

  static void read(CurtainState& settings, JsonObject& root) {
    root["led_on"] = settings.ledOn;
  }

  static StateUpdateResult update(JsonObject& root, CurtainState& curtainState) {
    boolean newState = root["led_on"] | DEFAULT_LED_STATE;
    if (curtainState.ledOn != newState) {
      curtainState.ledOn = newState;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

  static void haRead(CurtainState& settings, JsonObject& root) {
    root["state"] = settings.ledOn ? ON_STATE : OFF_STATE;
  }

  static StateUpdateResult haUpdate(JsonObject& root, CurtainState& curtainState) {
    String state = root["state"];
    // parse new led state 
    boolean newState = false;
    if (state.equals(ON_STATE)) {
      newState = true;
    } else if (!state.equals(OFF_STATE)) {
      return StateUpdateResult::ERROR;
    }
    // change the new state, if required
    if (curtainState.ledOn != newState) {
      curtainState.ledOn = newState;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class CurtainStateService : public StatefulService<CurtainState> {
 public:
  CurtainStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    CurtainMqttSettingsService* curtainMqttSettingsService);
  void begin();

 private:
  HttpEndpoint<CurtainState> _httpEndpoint;
  MqttPubSub<CurtainState> _mqttPubSub;
  WebSocketTxRx<CurtainState> _webSocket;
  AsyncMqttClient* _mqttClient;
  CurtainMqttSettingsService* _curtainMqttSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
