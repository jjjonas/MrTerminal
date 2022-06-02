#include <CurtainStateService.h>

CurtainStateService::CurtainStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient) :
    _httpEndpoint(CurtainState::read,
                  CurtainState::update,
                  this,
                  server,
                  CURTAIN_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(CurtainState::read,
                CurtainState::update,
                this,
                mqttClient,
                "mrcurtain/closing/set",
                "mrcurtain/closing/state"),
    _webSocket(CurtainState::read,
               CurtainState::update,
               this,
               server,
               CURTAIN_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient) {
      
  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void CurtainStateService::begin() {
  Serial.println("CurtainStateService::begin()");
  _state.closed = DEFAULT_CURTAIN_CLOSED_STATE;
  onConfigUpdated();
}

void CurtainStateService::onConfigUpdated() {
  Serial.println("CurtainStateService::onConfigUpdated()");
  // digitalWrite(LED_PIN, _state.closed ? LED_ON : LED_OFF);
}

