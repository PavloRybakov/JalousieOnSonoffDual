#ifndef __VIRTUALMOTOR_H
#define __VIRTUALMOTOR_H

#include "Ticker.h"

#include <stdlib.h>
#include <inttypes.h>
#include <functional>


class VirtualMotor {
  public:
    typedef std::function<void()> callback_t;

    /* Constructor.
     * Time in seconds, which is required
     * to close/open window jalousie.
     * (see maxDurationInMilliseconds).
     * 
     * And callback-function (without patameters),
     * which will be called if virtual motor stops.
     */
    VirtualMotor(unsigned int seconds, unsigned int waitForMotorStopMs, callback_t callback) : _callback(callback) {
        maxDurationInMilliseconds = seconds * 1000;
        spinStep = (float)(TICKER_TIME * 100) / (float)maxDurationInMilliseconds;
        if (waitForMotorStopMs > 1) {
            waitForMotorStop = waitForMotorStopMs;
        }
    }

    /** 
     * Call external method on virtual-motor interrupt-event:
     */
    void doCallbackOnInterrupt() {
      if (_callback) {
          _callback();
      } else {
          Serial.println("[Debug] ERROR: callback-method is NOT defined!");
      }
    }

    /* info methods: */
    bool isWorking();
    double getCurrentPosition();
    uint8_t getCurrentDirection();
    void debugInfo();
    /* control methods: */  
    void rotateUp();
    void rotateDown();
    void stopRotation();                                 /* Stop it from outside */

  private:
    const int _TOP          = 100;                       /* Top    Position: 100 % */
    const int _BOTTOM       = 0;                         /* Bottom Position:   0 % */

    /* call back organisation: */
    callback_t _callback;

    /* internal variables: */
    enum RotateDirection : uint8_t {UP, DOWN};
    RotateDirection currentDirecetion = UP;              /* As initial position means closed, we start rotate UP */

    volatile double currentPositionInPercent = _BOTTOM;  /* Initial position (closed) */
    double spinStep;                                     /* will be calculated in constructor */

    bool working = false;
    unsigned int maxDurationInMilliseconds = 0;          /* Minimal posible time, 
                                                          * which motor needs to open/close 
                                                          * virtual window jalousie.
                                                          */

    unsigned int waitForMotorStop = 1;                   /* Some motors need time to stop.
                                                          * Time in milliseconds
                                                          */
    
    void updatePosition();
    void callbackOnIterrup();
    void callibration();

    /* Internal timer stuff: */
    Ticker _ticker;
    static const int TICKER_TIME = 200;                  /* update virtual position each 0.2 sec. */

    /* ticker methods */
    void start() {
      working = true;
      _ticker.attach_ms(TICKER_TIME, tickerProc, this);
    }
    void stop() {
      working = false;
      _ticker.detach();
      delay(waitForMotorStop);
    }
    static void tickerProc(VirtualMotor *_this);
    
    /* desctructor */
    ~VirtualMotor() {
      stop();
    }
};
#endif
