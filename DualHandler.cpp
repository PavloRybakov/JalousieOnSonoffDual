#include "Arduino.h"
#include "DualHandler.h"

/**/
void DualHandler::init() {
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LED_OFF);
}

/**/
bool DualHandler::isRelayRedOn() {
   return relayRedIsOn;
}

/**/
bool DualHandler::isRelayGreenOn() {
   return relayGreenIsOn;
}

/**/
long DualHandler::getMillisSinceLastChange() {
  return millisSinceLastChange;
}

/**/
void DualHandler::readSerial() {
  if (Serial.available() >= 4) {
     unsigned char value0 = Serial.read();
     unsigned char value1 = Serial.read();
     unsigned char value  = Serial.read();
     unsigned char value3 = Serial.read();

     if (value0 == START_MODE) {
               
         if (value1 == READ_MODE || value1 == WRITE_MODE) {

            if (value3 == FINISH_MODE) {
               /* Debug:
               Serial.print(value0);
               Serial.print(":");
               Serial.print(value1);
               Serial.print(":");
               Serial.print(value);
               Serial.print(":");
               Serial.println(value3);
               Serial.flush();
               */
               
               if (value == _CLOSE) {  /*==1*/
                    relayRedIsOn   = true;
                    relayGreenIsOn = false;
               } 
               else if (value == _OPEN) { /*==2*/
                    relayRedIsOn   = false;
                    relayGreenIsOn = true;
               }
               else if (value == _BOTH) { /*==3*/
                    relayRedIsOn   = true;
                    relayGreenIsOn = true;
               }
               else {
                    /* value==0 {or unknown value} */
                    relayRedIsOn   = false;
                    relayGreenIsOn = false;
               }

               millisSinceLastChange = millis();
            }
         }
     }
  }
}

/**/
void DualHandler::setRelays(bool newStateRedOn, bool newStateGreenOn) {
  relayRedIsOn   = newStateRedOn;
  relayGreenIsOn = newStateGreenOn;

  unsigned char newState;
  if (newStateRedOn && !newStateGreenOn) {
     newState = _CLOSE; /* =1 */
  } else if ( !newStateRedOn && newStateGreenOn) {
     newState = _OPEN; /* =2 */
  } else if ( newStateRedOn && newStateGreenOn) {
     newState = _BOTH; /* =3 */
  } else {
     newState = _STOP; /* =0 */
  }

  writeSerial(newState);
}

/**/
void DualHandler::writeSerial(unsigned char newState) {
  Serial.flush(); /* clean from "trash" */
  
  Serial.write(START_MODE);
  Serial.write(WRITE_MODE);
  Serial.write(newState);
  Serial.write(FINISH_MODE);
  Serial.flush();

  millisSinceLastChange = millis();
}

/**/
void DualHandler::debugInfo() {
  Serial.print(relayRedIsOn ? "Red On" : "Red Off");
  Serial.print(" : ");
  Serial.println(relayGreenIsOn ? "Green On" : "Green Off");
  Serial.flush();
}

/**/
void DualHandler::ledBlink(int interval) {
  digitalWrite(BLUE_LED, LED_ON);
  delay(interval);
  digitalWrite(BLUE_LED, LED_OFF);
  delay(interval);
}

