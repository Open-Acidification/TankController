#include <Arduino.h>
#if 0
class pHControl_TC {
    private:
        const int CO2_REG_PIN = 49;
        long window_start_time = millis();

    public:
        // activate CO2 bubbler by opening solenoid
        void bubble() { digitalWrite(CO2_REG_PIN, LOW); }
        // deactivate CO2 bubbler by closing solenoid
        void noBubble() { digitalWrite(CO2_REG_PIN, HIGH); }
        
        void driveOutput();

};
#endif
