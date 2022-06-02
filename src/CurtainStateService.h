#ifndef CurtainStateService_h
#define CurtainStateService_h


#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define LED_PIN 2

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"


#define DEFAULT_CURTAIN_CLOSED_STATE false
#define OPEN_STATE "OPEN"
#define CLOSED_STATE "CLOSED"

// Note that the built-in LED is on when the pin is low on most NodeMCU boards.
// This is because the anode is tied to VCC and the cathode to the GPIO 4 (Arduino pin 2).
#define LED_ON 0x0
#define LED_OFF 0x1

#define CURTAIN_SETTINGS_ENDPOINT_PATH "/rest/curtainState"
#define CURTAIN_SETTINGS_SOCKET_PATH "/ws/curtainState"

class CurtainState {
 public:

  bool closed;

  static void read(CurtainState& settings, JsonObject& root) {
    root["closed"] = settings.closed;
  }

  static StateUpdateResult update(JsonObject& root, CurtainState& curtainState) {
    Serial.println("CurtainState::update()");

    boolean newState = root["closed"] | DEFAULT_CURTAIN_CLOSED_STATE;
    if (curtainState.closed != newState) {
      curtainState.closed = newState;
      Serial.print("CurtainState::update():: closed=");
      Serial.println(curtainState.closed);
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

};

class CurtainStateService : public StatefulService<CurtainState> {
 public:
  CurtainStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient);
  void begin();

 private:
  HttpEndpoint<CurtainState> _httpEndpoint;
  MqttPubSub<CurtainState> _mqttPubSub;
  WebSocketTxRx<CurtainState> _webSocket;
  AsyncMqttClient* _mqttClient;

  void onConfigUpdated();

};

#endif
