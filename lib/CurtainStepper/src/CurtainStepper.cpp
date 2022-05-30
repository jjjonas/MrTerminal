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

  // TODO: fix self caclulation
  setOpeningDistance(_cmFullMovement);

  if (_autoHomingAfterBoot) { startHomeingToEndstop(); }
}

void CurtainStepper::setOpeningDistance(double centimeters)
{
  _cmFullMovement = centimeters;
  _stepsFullMovement = _stepper->calcStepsForRotation((_cmFullMovement / _cmPerRound) * 360.0);
  Serial.print("setOpeningDistance():: ");
  Serial.print(_stepsFullMovement);
  Serial.println(" microsteps");
}

void CurtainStepper::startHomeingToEndstop()
{
  _status->setDelay(BLINK_CYLE_OPEN);
  if (digitalRead(_pinIn_Endstop) == ENDSTOP_REACHED)
  {
    Serial.println("startHomeingToEndstop(): already at ENDSTOP");
    _homingActive = false;
    _homed = true;
    _postition = 0;
  }
  else
  {
    Serial.println("startHomeingToEndstop(): start");    
    _homingActive = true;
    _stepper->enable();
    delay(5);
    _stepper->startMove(OPEN * _stepsFullMovement);
    delay(2);
  }
}

void CurtainStepper::_runHomeingToEndstop()
{    
  if (_homingActive)
  {      
    if (digitalRead(_pinIn_Endstop) == HIGH)
    {          
        Serial.println("STOPPER REACHED");

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
    Serial.print("startClosing():: ");  
    
    if (_postition == 0)
    {
      Serial.println("from ENDSTOP");
      _stepper->enable();
      delay(5);
      _stepper->startMove(CLOSE * _stepsFullMovement);
      _openCurtainActive = true;
    }
    else if (_postition > 0)
    {
      Serial.print("from position: "); 
      Serial.println(_postition);
      _stepper->enable();
      delay(5);
      _stepper->startMove(CLOSE * (_stepsFullMovement - _postition));
      _openCurtainActive = true;
    }
    else 
    {
      Serial.println(" homing needed");
      startHomeingToEndstop();
    }
}

void CurtainStepper::stopClosing()
{
  Serial.println("stopClosing()");
  _postition = (_stepsFullMovement - _stepper->getStepsRemaining());
  _stepper->stop();
  _openCurtainActive = false;
}

void CurtainStepper::toogleOpenCurtain()
{
  Serial.println("toogleOpenCurtain()");  
  if (_openCurtainActive)
  {
    stopClosing();
  }
  else
  {
    startClosing();
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

