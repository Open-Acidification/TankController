#pragma once

#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#define TankControllerTest TankController
#else
#define TankControllerProd TankController
#endif

class TankControllerProd {
    public:
        TankControllerProd();
        ~TankControllerProd();
        virtual String className() const { return "TankControllerProd"; }
        void setup();
        void loop();
    private:
        int foo;
};
