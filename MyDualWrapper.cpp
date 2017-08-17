#include "MyDualWrapper.h"
#include "DualHandler.h"

/**
 * Wrapper-class for the DualHandler.
 * Contains logic to serve the motor.
 **/
void MyDualWrapper::readSerial() {
    
    /* read serial and actualize logical relays states: */
    DualHandler::readSerial();

    /* implement the motor-logic: */
    if (isRelayRedOn() && isRelayGreenOn()) {
        /* two live-on can destroy the motor
         * ->
         * switch the motor off!
         */
        setRelays(false, false);
    }
}

