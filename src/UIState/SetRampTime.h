// /**
//  * SetRampTime.h
//  *
//  */
// #pragma once
// #include "NumberCollectorState.h"

// class SetRampTime : public NumCollectorState {
// public:
//   SetRampTime(TankController* tc);
//   virtual bool isInteger() {
//     return true;
//   }
//   const __FlashStringHelper* name() {
//     return F("SetRampTime");
//   }
//   float getCurrentValue();

//   const __FlashStringHelper* prompt() {
//     return prompts[subState];
//   }
//   void setValue(float value);

// private:
//   static const uint16_t NUM_VALUES = 3;
//   uint16_t subState = 0;
//   uint16_t values[NUM_VALUES] = {0, 0, 0};
//   float current[NUM_VALUES] = {0.0, 0.0, 0.0};
//   const __FlashStringHelper* prompts[NUM_VALUES];
// };
