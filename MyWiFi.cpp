#include <ESP8266WiFi.h>
#include "MyWiFi.h"

void MyWiFi::init() {
  WiFi.begin(SID, PWD);
}

bool MyWiFi::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}
