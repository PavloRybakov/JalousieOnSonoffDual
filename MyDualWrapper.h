#ifndef __MYDUALWRAPPER_H
#define __MYDUALWRAPPER_H

#include "DualHandler.h"

class MyDualWrapper : public DualHandler {
  public:
    void readSerial();
};
#endif
