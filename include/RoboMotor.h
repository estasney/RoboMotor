#ifndef ROBOMOTOR_ROBOMOTOR_H
#define ROBOMOTOR_ROBOMOTOR_H

#include "Arduino.h"

/**
 * @brief Motor Manager
 *
 * Motor Manager that manages state
 * @param pinA
 *   pin number A
 * @param pinB
 *   pin number B
 * @param pinPulse
 *   pin number for pulse
 * @param leftSide
 *   if true, the motor is located on the left side
 * @param decayX
 *   rate of decay for X vector. I.e. motor will stop faster the higher this value is
 * @param decayY
 *   rate of decay for Y vector. I.e. motor will stop faster the higher this value is
 */
class RoboMotor {
public:
    RoboMotor(uint8_t pinA, uint8_t pinB, uint8_t pinPulse, bool leftSide, uint8_t decayX, uint8_t decayY);  // Constructor
    void begin();
    /**
     * @brief Forward
     * Set motor to go forward
     */
    void forward();
    /**
     * @brief Brake
     * Set motor to brake
     */
    void brake();
    /**
     * @brief Reverse
     * Set motor to reverse
     */
    void reverse();

    /**
     * @brief Update Throttle with new Values
     *
     * These are debounced
     * @param deltaX
     * @param deltaY
     */
    void updateThrottle(int deltaX, int deltaY);
    /**
     * @return current speed
     */
    int getSpeed();
    /**
     * @brief Tick to apply decay
     * @param tickX
     * @param tickY
     */
    void tick(bool tickX, bool tickY);


private:
    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _pinPulse;
    int _pinAState;
    int _pinBState;
    int _pulseState;
    int16_t _vecX;
    int16_t _vecY;
    bool _leftSide;
    int _decayX;
    int _decayY;

    void _writeLogicPins() const;

    void _writeThrottle();

    void _setThrottle();

};


#endif //ROBOMOTOR_ROBOMOTOR_H