#include "Arduino.h"
#include "VirtualMotor.h"

/**/
void VirtualMotor::tickerProc(VirtualMotor *_this) {
    _this->updatePosition();
}

/**/
bool VirtualMotor::isWorking() {
  return this->working;
}

/**/
double VirtualMotor::getCurrentPosition() {
  return this->currentPositionInPercent;
}

/**/
uint8_t VirtualMotor::getCurrentDirection() {
  return this->currentDirecetion;
}

/**
 * Stop it from outside:
 */
void VirtualMotor::stopRotation() {
  this->stop();
}

/**/
void VirtualMotor::updatePosition() {
  /*
    Serial.print("[DEBUG] Enter updatePosition. Step: ");
    Serial.print(spinStep);
    Serial.print(". Position=");
    Serial.println(currentPositionInPercent);
  */
    if ( getCurrentDirection() == UP ) {
        currentPositionInPercent += (float)spinStep;
    } else {
        currentPositionInPercent -= (float)spinStep;
    }

    if (getCurrentPosition() > _TOP) {
        this->stop();
        currentPositionInPercent = _TOP;
        this->doCallbackOnInterrupt();
    } else 
    if (getCurrentPosition() < _BOTTOM) {
        this->stop();
        currentPositionInPercent = _BOTTOM;
        this->doCallbackOnInterrupt();
    }
}

/**/
void VirtualMotor::rotateUp() {
    if (isWorking()) {
        /* if still working -> stop it */
        this->stop();
    }
    currentDirecetion = UP;
    this->start();
}

/**/
void VirtualMotor::rotateDown() {
    if (isWorking()) {
        /* if still working -> stop it */
        this->stop();
    }
    currentDirecetion = DOWN;
    this->start();
}

/**
 * Has to be implemented later...
 * 
 * Has to be started after class initialisation.
 * Initial position (see currentPositionInPercent) ) = _BOTTOM
 * So, method just starts rotateUp till the end:
 */
void VirtualMotor::callibration() {
   /* TODO  */
}

/**/
void VirtualMotor::debugInfo() {
    if (isWorking()) {
      Serial.print("State: WORKING , Direction:");
      Serial.print( (getCurrentDirection() == UP) ? " UP " : " DOWN ");
    } else {
      Serial.print("State: WAITING ");
    }
    Serial.print(" -> cur.position: ");
    Serial.print(getCurrentPosition());
    Serial.println(" %");
    Serial.flush();
}
