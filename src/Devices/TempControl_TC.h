
// class temp control -temp setpoint +get/set temp target +virtual updateCtrl(),  subclass heater (respective code) and chiller (respective code)
class TempControl_TC {
private:
  const int CHILLER_PIN = 47;
  const unsigned long CHILLER_INTERVAL = 30 * SECOND_IN_MILLIS;                 // interval at which to change chiller state

  unsigned long currentTime = millis();
  unsigned long previousTime = 0;
  double tempTarget;
  

public:
  TempControl_TC() {
  }

  void setTempTarget();
  void getTempTarget();
  virtual void updateCtrl();
  
};

class Heater_TC : public TempControl_TC {
  private:

  public:
    void heat();
};

class Chiller_TC : public TempControl_TC {
  private:

  public:
    void chill();

}