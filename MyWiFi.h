#ifndef __MYWIFI_H
#define __MYWIFI_H

/* WiFi information:*/
const char SID[] = "Wlan-Name";
const char PWD[] = "Password-for-Your-Wlan";

class MyWiFi {
  public:
      void init();
      bool isConnected();
};
#endif

