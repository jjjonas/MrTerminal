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
  
  long __newStepsFullMovement = _stepper->calcStepsForRotation((_cmFullMovement / _cmPerRound) * 360.0);
  long __dif =   __newStepsFullMovement - _stepsFullMovement; // + = longer | - = shorter


  if (abs(__dif) < 5)
  {
    /* Do nothing */;
  }
  else if (__dif > 0)
  {
    /* way to close is now longer */
    _dirCommand = MovementDirection::DIR_CLOSE;
    _movState = Movement::CLOSING;
    _stepsFullMovement = __newStepsFullMovement;
  }
  else if (__dif < 0)
  {
    /*        pos                            */
    /* <-------|-------------------------->  */
    /*         |     <-------------------->  */

    if (_position > __newStepsFullMovement)
    {    
    /* was to close is now shorter */
    /*               pos                     */
    /*               <|------------------->  */

      _dirCommand = MovementDirection::DIR_CLOSE;
      _movState = Movement::CLOSED;
      _position = __newStepsFullMovement;
    }
    else
    {
      _invalidateHomed();
    }
    _stepsFullMovement = __newStepsFullMovement;
  }


  Serial.print("setOpeningDistance():: update (old/new) ");
  Serial.print(_stepsFullMovement);
  Serial.print("/");
  Serial.print(__newStepsFullMovement);

  
}

void CurtainStepper::startHomeingToEndstop()
{
  _status->setDelay(BLINK_CYLE_OPEN);
  if (digitalRead(_pinIn_Endstop) == ENDSTOP_REACHED)
  {
    Serial.println("startHomeingToEndstop(): already at ENDSTOP");
    _homingActive = false;
    _homed = true;
    _position = 0;
    _movState = Movement::OPEN;
  }
  else
  {
    Serial.println("startHomeingToEndstop(): start");    
    _homingActive = true;
    _stepper->enable();
    delay(5);
    _stepper->startMove(DIRECTION_OPEN * _stepsFullMovement);
    delay(2);
  }
}

void CurtainStepper::_runHomeingToEndstop()
{    
  if (_homingActive)
  {      
    if (digitalRead(_pinIn_Endstop) == ENDSTOP_REACHED)
    {          
        Serial.println("STOPPER REACHED");

        _stepper->stop();              
        _homingActive = false;
        _homed = true;
        _position = 0;
        _movState = Movement::OPEN;
    }
  }
}

void CurtainStepper::_invalidateHomed()
{
  Serial.println("_invalidateHomed()::");
  _homingActive = false;
  _homed = false;
  _position = -1;
  _movState = Movement::STOPPED;
}

void CurtainStepper::startClosing()
{
    _status->setDelay(BLINK_CYLE_OPEN);
    Serial.print("startClosing():: ");  
    _dirCommand = MovementDirection::DIR_OPEN;
    if (_position == 0)
    {
      Serial.println("from ENDSTOP");
      _stepper->enable();
      delay(5);
      _stepper->startMove(DIRECTION_CLOSE * _stepsFullMovement);
      _movState = CLOSING;
    }
    else if (_position > 0)
    {
      Serial.print("from position: "); 
      Serial.println(_position);
      _stepper->enable();
      delay(5);
      _stepper->startMove(DIRECTION_CLOSE * (_stepsFullMovement - _position));
      _movState = CLOSING;
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
  _position = (_stepsFullMovement - _stepper->getStepsRemaining());
  _stepper->stop();
  _movState = STOPPED;
}

void CurtainStepper::startOpening()
{
    _status->setDelay(BLINK_CYLE_OPEN);
    Serial.print("startOpening():: ");  
    _dirCommand = MovementDirection::DIR_OPEN;
    if (_position == 0)
    {
      Serial.println("still on ENDSTOP");
    }
    else if (_position > 0)
    {
      Serial.print("from position: "); 
      Serial.println(_position);
      _stepper->enable();
      delay(5);
      _stepper->startMove(DIRECTION_OPEN * (_position));
      _movState = OPENING;
    }
    else 
    {
      Serial.println(" homing needed");
      startHomeingToEndstop();
    }
}

void CurtainStepper::stopOpening()
{
  Serial.println("stopOpening()");
  _position = (_stepsFullMovement + _stepper->getStepsRemaining());
  _stepper->stop();
  _movState = STOPPED;
}

void CurtainStepper::stop()
{
  Serial.println("stop()");
  switch (_movState)
  {
  case Movement::CLOSING:
    stopClosing();
    break;
  case Movement::OPENING:
    stopOpening();
    break;
  default:
    break;
  }
}

void CurtainStepper::toogleOpenCurtain()
{
  Serial.println("toogleOpenCurtain()");  
  
  switch (_movState)
  {
  case Movement::OPEN:
    startClosing();
    break;
  case Movement::CLOSED:
    startOpening();
    break;
  case Movement::OPENING:
    stopOpening();
    break;
  case Movement::CLOSING:
    stopClosing();
    break;
  case Movement::STOPPED:
    switch (_dirCommand)
    {
    case MovementDirection::DIR_CLOSE:
      startOpening();
      break;
    case MovementDirection::DIR_OPEN:
      startClosing();
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void CurtainStepper::tick()
{
  
  _runHomeingToEndstop();
  
  // motor control loop - send pulse and return how long to wait until next pulse

  if (digitalRead(_pinIn_Endstop) == ENDSTOP_REACHED) 
  {
    stop();
    startHomeingToEndstop();
  }
  
  unsigned wait_time_micros = _stepper->nextAction();

  // 0 wait time indicates the motor has stopped
  if (wait_time_micros <= 0) {
  _stepper->disable();       // comment out to keep motor powered
  _status->setDelay(BLINK_CYLE_SLEEP_ON, BLINK_CYLE_SLEEP_OFF);
  delay(1);     
  

  /* call status blinker */
  if (enableStatusLED)
  {
    _status->blink();
  }

}

