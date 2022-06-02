#ifndef CurtainStateService_h
#define CurtainStateService_h

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define DEFAULT_CURTAIN_CLOSED_STATE false
#define OPEN_STATE "OPEN"
#define CLOSED_STATE "CLOSED"

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
