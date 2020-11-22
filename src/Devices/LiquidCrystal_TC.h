#include <Arduino.h>
#ifdef MOCK_PINS_COUNT
#include <LiquidCrystal_CI.h>
#else
#include <LiquidCrystal.h>
#endif

class LiquidCrystal_TC {
private:
  const int RS = 24, EN = 22, D4 = 26, D5 = 28, D6 = 30, D7 = 32;
  LiquidCrystal* pLCD;  // (RS, EN, D4, D5, D6, D7);

public:
  LiquidCrystal_TC();
  ~LiquidCrystal_TC();
  void clear();
  void setCursor();
  void print();
};
