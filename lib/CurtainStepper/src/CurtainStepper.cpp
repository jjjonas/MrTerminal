/*
  CurtainStepper Library for Curtain moving curains

  Copyright (c) 2022 Jonas Jungjohann. All rights reserved.
  
*/

#include "Arduino.h"
#include "CurtainStepper.h"


CurtainStepper::CurtainStepper()
{
}
CurtainStepper::CurtainStepper(A4988& stepper, Blinker& blinker, const int pin_endstop)
{
  _stepper = &stepper;
  _status = &blinker;
  _pinIn_Endstop = pin_endstop;
}

void CurtainStepper::begin()
{
  // configure endstop
  pinMode(_pinIn_Endstop, INPUT_PULLUP);

  _status->setDelay(BLINK_CYLE_SLEEP_ON, BLINK_CYLE_SLEEP_OFF);
	_status->start();

  _stepper->begin(_rpm, MICROSTEPS);  
  // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
  _stepper->setEnableActiveState(LOW);

  _stepper->setSpeedProfile(_stepper->LINEAR_SPEED, _motor_Acceleration, _motor_Deceleration);

}

void CurtainStepper::startHomeingToEndstop()
{
    Serial.println("startHomeingToEndstop():");
    if (digitalRead(_pinIn_Endstop) == ENDSTOP_REACHED)
    {
      Serial.println("startHomeingToEndstop(): already at ENDSTOP");      
    }
    else
    {
      _status->setDelay(BLINK_CYLE_CLOSE);
      _homingActive = true;
      _stepper->enable();
      delay(10);
      _stepper->startRotate(-_turnsToOpent * 2 * 360);
      delay(5);
    }
}

void CurtainStepper::_runHomeingToEndstop()
{    
    if (_homingActive)
    {      
      if (digitalRead(_pinIn_Endstop) == HIGH)
      {          
          Serial.println("STOPPER REACHED");

          /*
          * Choosing stop() vs startBrake():
          *
          * constant speed mode, they are the same (stop immediately)
          * linear (accelerated) mode with brake, the motor will go past the stopper a bit
          */

          _stepper->stop();              
          _homingActive = false;
          _homed = true;
          _postition = 0;
      }
    }
}

void CurtainStepper::startClosing()
{
    _status->setDelay(BLINK_CYLE_OPEN);
    Serial.println("startClosing()");  
    _stepper->enable();
    _stepper->startRotate(_turnsToOpent * 360);
    _openCurtainActive = true;
}

void CurtainStepper::stopClosing()
{
  Serial.println("stopClosing()");    
  _stepper->startBrake();
  _openCurtainActive = false;
}

void CurtainStepper::toogleOpenCurtain()
{
  Serial.println("toogleOpenCurtain()");  
  if (!_homed)
  {
    Serial.println("toogleOpenCurtain():: not homed, first do homing.");
    startHomeingToEndstop();
  }
  else
  {
    if (_openCurtainActive)
    {
      stopClosing();
    }
    else
    {
      startClosing();
    }
  }
}

void CurtainStepper::tick()
{
  
  _runHomeingToEndstop();
  
  // motor control loop - send pulse and return how long to wait until next pulse
  unsigned wait_time_micros = _stepper->nextAction();
  
  // 0 wait time indicates the motor has stopped
  if (wait_time_micros <= 0) {
      _stepper->disable();       // comment out to keep motor powered
      _status->setDelay(BLINK_CYLE_SLEEP_ON, BLINK_CYLE_SLEEP_OFF);
      delay(1);        
  }
  
  if (enableStatusLED)
  {
    _status->blink();
  }

}

