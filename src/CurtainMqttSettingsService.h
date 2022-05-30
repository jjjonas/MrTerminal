#ifndef CurtainMqttSettingsService_h
#define CurtainMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <SettingValue.h>

#define CURTAIN_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define CURTAIN_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class CurtainMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void read(CurtainMqttSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static StateUpdateResult update(JsonObject& root, CurtainMqttSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | SettingValue::format("homeassistant/curtain/#{unique_id}");
    settings.name = root["name"] | SettingValue::format("curtain-#{unique_id}");
    settings.uniqueId = root["unique_id"] | SettingValue::format("curtain-#{unique_id}");
    return StateUpdateResult::CHANGED;
  }
};

class CurtainMqttSettingsService : public StatefulService<CurtainMqttSettings> {
 public:
  CurtainMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<CurtainMqttSettings> _httpEndpoint;
  FSPersistence<CurtainMqttSettings> _fsPersistence;
};

#endif  // end CurtainMqttSettingsService_h
