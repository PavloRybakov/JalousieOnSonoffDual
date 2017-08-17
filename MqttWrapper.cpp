#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MqttWrapper.h"

WiFiClient espClient;
PubSubClient client(espClient);

/**
 */
MqttWrapper& MqttWrapper::setCallback(std::function<void(char*, byte*, unsigned int)> callback) {
  client.setCallback(callback);
  return *this;
}

/**
 */
void MqttWrapper::init() {
  client.setServer(server.c_str(), port);
}

/**
 */
bool MqttWrapper::isConnected() {
  return client.connected();
}

/**
 */
bool MqttWrapper::connect() {
  bool isconnected = client.connect(serviceName.c_str(), user.c_str(), password.c_str());
  if (isconnected) {
      client.subscribe(topicState.c_str()); 
  }
  return isconnected;
}

/**
 */
void MqttWrapper::publishState(String newValue) {
    client.publish(topicState.c_str(), newValue.c_str(), true);
}

/**
 */
void MqttWrapper::publishPosition(String newValue) {
  client.publish(topicPosition.c_str(), newValue.c_str(), true);
}

/**
 */
void MqttWrapper::lifeCycle() {
  client.loop();
}

/**
 */
void MqttWrapper::debug(String msg) {
  Serial.println(msg);
  Serial.flush();
}

/** 
 * Handle MQTT-callback: 
 *
void MqttWrapper::callback(char* topic, byte* payload, unsigned int length) { 

    /
    String strPayload = String((char*)payload);
    String strTopic = String((char*)topic);
    if(strcmp(strTopic, "audio/home/bedroom/title") == 0) {
        ....
    }
    /
    
    /
    if ((char)payload[0] == '0') {
        desiredRelayState = 0;
    } else 
    /
    if ((char)payload[0] == '1') {
        desiredRelayState = 1;
    } else 
    if ((char)payload[0] == '2') {
        desiredRelayState = 2;
    } else {
        desiredRelayState = 0;
    }
}
*/
