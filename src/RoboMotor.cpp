#include "Arduino.h"
#include "RoboMotor.h"

RoboMotor::RoboMotor(uint8_t pinA, uint8_t pinB, uint8_t pinPulse, bool leftSide, uint8_t decayX, uint8_t decayY)
{
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinPulse, OUTPUT);
    _pinA = pinA;
    _pinB = pinB;
    _pinPulse = pinPulse;
    _pinAState = LOW;
    _pinBState = LOW;
    _pulseState = 0;
    _vecX = 0;
    _vecY = 0;
    _leftSide = leftSide;
    _decayX = decayX;
    _decayY = decayY;
}

void RoboMotor::_writeLogicPins() const{
    digitalWrite(_pinA, _pinAState);
    digitalWrite(_pinB, _pinBState);
}

void RoboMotor::_writeThrottle(){
    analogWrite(_pinPulse, abs(_pulseState));
}

void RoboMotor::_setThrottle()
{
    _pulseState = constrain((_vecX + _vecY), -255, 255);

    if (_pulseState < 0) {
        reverse();
    } else if (_pulseState == 0) {
        brake();
    } else {
        forward();
    }
    _writeThrottle();
}

void RoboMotor::updateThrottle(int16_t deltaX, int16_t deltaY) {

    _vecY += deltaY;

    /*
     Skid Steering

     deltaX | direction | Action  | Sign
     ------   ---------   -------   -----
       +         R        L > R      + if _leftSide else -
       -         L        L < R      - if _leftSide else +
       0         NA       L == R     0

    */

    if (_leftSide) {
        _vecX += deltaX;
    } else {
        _vecX -= deltaX;
    }

    _vecX = constrain(_vecX, -255, 255);
    _vecY = constrain(_vecY, -255, 255);

    _setThrottle();
}

void RoboMotor::begin()
{
    pinMode(_pinA, OUTPUT);
    pinMode(_pinB, OUTPUT);
    pinMode(_pinPulse, OUTPUT);
    _writeLogicPins();
    analogWrite(_pinPulse, _pulseState);
}

void RoboMotor::forward()
{
    _pinAState = LOW;
    _pinBState = HIGH;
    _writeLogicPins();
}

void RoboMotor::reverse()
{
    _pinAState = HIGH;
    _pinBState = LOW;
    _writeLogicPins();
}

void RoboMotor::brake()
{
    _pinAState = LOW;
    _pinBState = LOW;
    _writeLogicPins();
}

int RoboMotor::getSpeed()
{
    return _pulseState;
}

void RoboMotor::tick(bool tickX, bool tickY)
{

    if (!tickX && !tickY) {
        return;
    }

    int deltaX;
    int deltaY;

    if (tickX) {
        deltaX = _vecX / _decayX;
        if (deltaX == 0) {
            _vecX = 0;
        }
    } else {
        deltaX = 0;
    }


    if (tickY) {
        deltaY = _vecY / _decayY;
        if (deltaY == 0) {
            _vecY = 0;
        }
    } else {
        deltaY = 0;
    }

    _vecX -= deltaX;
    _vecY -= deltaY;

    _setThrottle();

}

