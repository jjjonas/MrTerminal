/*
  CurtainStepper Library for Curtain moving curains

  Copyright (c) 2022 Jonas Jungjohann. All rights reserved.
  
*/

#include "Blinker.h"
#include "A4988.h"

#ifndef CURTAINSTEPPER_H_
#define CURTAINSTEPPER_H_

#ifndef MICROSTEPS
// Microstepping mode. If you hardwired it to save pins, set to the same value here.
#define MICROSTEPS 16
#endif

#ifndef ENDSTOP_REACHED
// Define detection logic of endstop switch
#define ENDSTOP_REACHED 0x1
#endif

#ifndef DEFAULT_OPENING_DISTANCE_CM
#define DEFAULT_OPENING_DISTANCE_CM 66.0
#endif


#define BLINK_CYLE_SLEEP_ON 5000
#define BLINK_CYLE_SLEEP_OFF 100
#define BLINK_CYLE_OPEN 1000
#define BLINK_CYLE_CLOSE 100

#define OPEN -1
#define CLOSE 1

class CurtainStepper;

class CurtainStepper {
  
  private:
  A4988* _stepper = nullptr;
  Blinker* _status = nullptr;
  
  protected:

  int _pinIn_Endstop = -1;

  // Target RPM for cruise speed
  float _rpm = 60.0;
  
  double _cmPerRound = 3.7699;      //cm
  double _cmFullMovement = DEFAULT_OPENING_DISTANCE_CM;    //cm
  long   _stepsFullMovement = 0;      //steps

  // Acceleration and deceleration values are always in FULL steps / s^2
  short _motor_Acceleration = 150;
  short _motor_Deceleration = 500;

  /*
   * actual position in microsteps
   */
  int _postition = -1;
  
  bool _autoHomingAfterBoot = false;

  bool _homed = false;
  bool _homingActive = false;
  bool _openCurtainActive = false;

  void _runHomeingToEndstop();

  

  public:
  CurtainStepper();
  CurtainStepper(A4988& stepper, Blinker& blinker, const int pin_endstop = -1);


  bool enableStatusLED = true;

  void begin();
  void startHomeingToEndstop();
  void startClosing();
  void stopClosing();
  

  void toogleOpenCurtain();

  void setOpeningDistance(double centimeters);
  
  void tick();

};


#endif /* CURTAINSTEPPER_H_ */