#include <CurtainMqttSettingsService.h>

CurtainMqttSettingsService::CurtainMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(CurtainMqttSettings::read,
                  CurtainMqttSettings::update,
                  this,
                  server,
                  CURTAIN_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(CurtainMqttSettings::read, CurtainMqttSettings::update, this, fs, CURTAIN_BROKER_SETTINGS_FILE) {
}

void CurtainMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
