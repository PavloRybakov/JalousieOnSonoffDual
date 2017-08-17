#ifndef __MQTTWRAPPER_H
#define __MQTTWRAPPER_H

#include <functional>

class MqttWrapper {
	public:
    void init();
    bool isConnected();
    bool connect();
    void publishState(String newValue);
    void publishPosition(String newValue);
    void lifeCycle();
    MqttWrapper& setCallback(std::function<void(char*, byte*, unsigned int)> callback);

	private:
		/* MQTT information: */
    const String server       = "192.168.xx.xx";
		const int    port         = 1883;
		const String user         = "***********";
		const String password     = "***********";
		const String topicState   = "home/kitchen/leftwindow/motor/state";
		const String topicPosition= "home/kitchen/leftwindow/motor/position";
		const String serviceName  = "esp8285:" + topicState;

    /*void callback(char* topic, byte* payload, unsigned int length);*/

    int   relayState = 0;
    int   desiredRelayState = 0;

    void debug(String msg);
};
#endif
