#pragma once

#include "TankControllerLib.h"
#ifdef MOCK_PINS_COUNT

class TankControllerTest : public TankControllerProd {
    public:
        TankControllerTest();
        ~TankControllerTest();
        virtual String className() const { return "TankControllerTest"; }
        void setup();
        void loop();
    private:
        int foo;
};

#endif
