/**************************************************************
 * 
 * Pavlo Rybakov
 * Jul.2017
 * 
 * Scatch allows 
 * < or has to allow :-) >
 * to monitor and control motor for external jalousie,
 * using Sonoff Dual.
 * 
 * Expected motor: usual tube-motor with 2 life-inputs.
 * For this reason MyDualWrapper-class tries to stop 
 * the motor as soon as both relays are on.
 * 
 * Virtual-Motor:
 * class has to calculate current jalousie position.
 * Triggers "stop" for both relays if max/min jalousie position reached.
 * 
 * MQTT: 
 *  - publish/subscribe actual relays state
 *  - publish calculated jalousie position
 * 
 * p.s. No warranty
 * 
 **************************************************************/

#include "MyWiFi.h"
#include "MyDualWrapper.h"
#include "MqttWrapper.h"
#include "VirtualMotor.h"

#include "Ticker.h"

#define VIRTUAL_MOTOR_TIMEOUT 23      /* 23 seconds need motor to close/open the window */


#define READ_F330_TIMEOUT_MS 200      /* 5 times per second */
#define PUBLISH_POSITION_DELAY 500    /* publish new position min. 2 times per second (if virtual motor is running) */
volatile unsigned long millisSinceLastPositionPublished = 0;

/***/
MyWiFi myWifi;
MqttWrapper myMqtt;
MyDualWrapper dualHandler;
VirtualMotor *virtualMotor;

Ticker ticker;

/* for debug reasons: */
volatile unsigned long millisDebug = 0;

/**
*/
void readSerialFromF330() {
  //* remember current state:
  bool relay1On = dualHandler.isRelayRedOn();
  bool relay2On = dualHandler.isRelayGreenOn();

  //* actualize state:
  dualHandler.readSerial();

  //* compare new state:
  bool same = (relay1On == dualHandler.isRelayRedOn() && relay2On == dualHandler.isRelayGreenOn());
  if (!same) {
      /* mqtt state shold not have 
       * the same logic as dual,
       * but we take it over (for the moment):
       * 
       * 0 - both relays are off
       * 1 - one is on
       * 2 - second is on
       */
       String newValue = "0";
       if ( dualHandler.isRelayRedOn() && dualHandler.isRelayGreenOn() ) {
          newValue = String(dualHandler._BOTH);  /* =="3" <- error */
       } else
       if ( dualHandler.isRelayRedOn() ) {
          newValue = String(dualHandler._CLOSE); /* =="1" */
       } else
       if ( dualHandler.isRelayGreenOn() ) {
          newValue = String(dualHandler._OPEN);  /* =="2" */
       }

        //* run motor-shadow object:
        changeVirtualMotorState(dualHandler.isRelayRedOn(), dualHandler.isRelayGreenOn());

       //* publish new state:
       myMqtt.publishState(newValue);
  }
}

/**
 */
void writeSerialToF330(char* topic, byte* payload, unsigned int length) { 

    //* remember current state:
    bool relay1On = dualHandler.isRelayRedOn();
    bool relay2On = dualHandler.isRelayGreenOn();

    bool reset = false;

    if ((char)payload[0] == '1') {
        if ( !relay1On ) {
            relay1On = true;
            reset = true;
        }
    } else 
    if ((char)payload[0] == '2') {
        if ( !relay2On ) {
            relay2On = true;
            reset = true;
        }
    } else {
        relay1On = false;
        relay2On = false;
        reset = true;
    }

    if (reset) {
      
        //* run motor-shadow object:
        changeVirtualMotorState(relay1On, relay2On);

        //* say to F330:
        dualHandler.setRelays(relay1On, relay2On);
    }
}

/**
 */
void changeVirtualMotorState(bool relayRedIsOn, bool relayGreenIsOn) {

      if (relayGreenIsOn) {
          virtualMotor->rotateUp();
      } else 
      if (relayRedIsOn) {
          virtualMotor->rotateDown();
      } else {
          virtualMotor->stopRotation();
      }
}

/**
 */
void callbackOnVMInterrupt() {

    //* Step.1: say "stop" to F330
    dualHandler.setRelays(false, false);

    //* Step.2: publish "stop"-state to mqtt:
    myMqtt.publishState( String(dualHandler._STOP) );
}

/**
*/
void setup() {
  Serial.begin(19200);

  dualHandler.init();
  
  ticker.attach_ms(READ_F330_TIMEOUT_MS, readSerialFromF330);

  myWifi.init();

  myMqtt.init();
  myMqtt.setCallback(writeSerialToF330);

  /* For debug reasons.
   * Remove it in final version:
   */
  delay(5000);  
  Serial.println("Init Motor: ");

  virtualMotor = new VirtualMotor(VIRTUAL_MOTOR_TIMEOUT, 1, std::bind(&callbackOnVMInterrupt));
}

/**
*/
void loop() {

  if ( ! myWifi.isConnected() ) {
    
    //* wifi is still not connected:
    dualHandler.ledBlink(50);
    delay(300);
  } else if ( ! myMqtt.isConnected() ) {
    
    //* flash with led 2 times
    dualHandler.ledBlink(50);
    dualHandler.ledBlink(50);
    //* and try to reconnect:
    myMqtt.connect();
  } else {
    
    //* just let it work:
    myMqtt.lifeCycle();
  }

  if ( myMqtt.isConnected() ) {
    if (virtualMotor->isWorking()) {
        if (millis() - millisSinceLastPositionPublished > PUBLISH_POSITION_DELAY) {
            //* publish current position:
            int newposition = (int)virtualMotor->getCurrentPosition();
            myMqtt.publishPosition(String(newposition));
            //* public rotation direction:
            /* TODO */
            millisSinceLastPositionPublished = millis();
        }
    }
  }

  if (millis() - millisDebug > 1000) {
     dualHandler.debugInfo();
     virtualMotor->debugInfo();
     millisDebug = millis();
  }
}

