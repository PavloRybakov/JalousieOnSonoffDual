#ifndef __DUALHANDLER_H
#define __DUALHANDLER_H

#include <inttypes.h>

/*
0x00 both are off
0x01 relay one on
0x02 relay two on
0x03 both relays on
*/

class DualHandler {
  public:
    const uint8_t _STOP  = 0;
    const uint8_t _CLOSE = 1; /* relay with RED led I use for logical "close"-action */
    const uint8_t _OPEN  = 2; /* relay with GREEN led I use for logical "open"-action */
    const uint8_t _BOTH  = 3; /* support this state */

    void init();
    void readSerial();
    void setRelays(bool newStateRedOn, bool newStateGreenOn);
    bool isRelayRedOn();
    bool isRelayGreenOn();
    void debugInfo();
    long getMillisSinceLastChange();
    void ledBlink(int interval);

  private:
    const unsigned char START_MODE  = 0xA0;
    const unsigned char READ_MODE   = 0x00; /* usual 0x04 */
    const unsigned char WRITE_MODE  = 0x04;
    const unsigned char FINISH_MODE = 0xA1;

    const int BLUE_LED  = 13;
    const int LED_ON    = 0;
    const int LED_OFF   = 1;

    unsigned long millisSinceLastChange = 0;

    bool relayRedIsOn   = false;
    bool relayGreenIsOn = false;

    void writeSerial(unsigned char newState);
};
#endif
