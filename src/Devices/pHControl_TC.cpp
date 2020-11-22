#include "pHControl_TC.h"

// Should be called by ISR every 15 ms
void pHControl_TC::driveOutput() {
    long now = millis();
    // Set the output
    // "on time" is proportional to the PID output
    if (now - window_start_time > WINDOW_SIZE) {  // time to shift the Relay Window
        window_start_time += WINDOW_SIZE;
    }
    // on_time may need to be passed in
    if ((on_time > 100) && (one_time > (now - window_start_time))) {
        bubble();
    } else {
        noBubble();
    }
}
