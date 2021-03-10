/*!
 * @file Feedback360Servo.h
 * 
 * This library is not yet finished.
 * 
 * This library is for controlling a 4 wire 360 deg. servo motor which provides PWM feedback.
 * The PWM feedback prives a PWM input at 910 Hz with a 2.7% to 97.1% duty cycle.
 * The duty cycle can be calculated as 100% * (tHigh / tCycle).
 * The angular position can be calculated as (duty cycle - duty cycle min) * units for full circle / (duty cycle max - duty cycle min + 1)
 * Informatin based on the documentatino for the Parallax Feedback 360 deg High-Speed Servo (#900-00360)
 * 
 * @author Garabed Simitian. garabed.simitian.893@my.csun.edu
 */


#if defined (ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
    #include <PID_v1.h>
#else
    #error The Feedback360Servo library is only supported on Arduino version 1.0 and later
#endif
#include <Servo.h>

#ifndef FEEDBACK360SERVO_H
#define FEEDBACK360SERVO_H
#define LIBRARY_VERSIN  1.0

namespace feedback_servo{

    #define MIN_DUTY_CYCLE  2.9
    #define MAX_DUTY_CYCLE  97.1

    #define FEEDBACK_SIGNAL_FREQUENCY_HZ   910
    #define T_CYCLE (1.0 / FEEDBACK_SIGNAL_FREQUENCY_HZ)

    #define MAX_SPEED_CLOCKWISE_US          1280
    #define MAX_SPEED_COUNTERCLOCKWISE_US   1720
    #define MAX_ZERO_SPEED_US               1520
    #define MIN_ZERO_SPEED_US               1480
    #define ZERO_SPEED_US                   1500

    #define ABS_MAX_SPEED_RPM               140
    #define MAX_SPEED_CLOCKWISE_RPM         140
    #define MAX_SPEED_COUNTERCLOCKWISE_RPM  -140

    #define ABS_MAX_SPEED_WITH_FEEDBACK_RPM                     120
    #define MAX_SPEED_CLOCKWISE_WITH_FEEDBACK_RPM               120
    #define MAX_SPEED_COUNTERCLOCKWISE_WITH_FEEDBACK_RPM        -120

    #define CLOCKWISE           1
    #define COUNTERCLOCKWISE    -1

    /*! 
    * @class Feedback360Servo
    * @brief A servo class built on top of the Arduino Servo library to controll a 4 wire 360 deg. servo providing PWM feedback.
    * This class does not inherit from the Servo class in the Arduino Servo library.
    * Intended for the Parallax Feedback 360 deg. continus rotating servo. 
    * The only available constructor is the default constructor. 
    * The bigin(int, int) function must be called to set the control pin and feedback pin.
    */
    class Feedback360Servo{
        private:

        int feedback_pin_num;
        Servo feedback_servo;

        public:
        /*! 
        * Set up the Feedback360Servo object.
        * Must be called before other functions are called with the exception of the attach(int) attachFeedbackPin(int).
        * If not called before calling other function, the other functions will not work properly.
        * @param attach_pin Pin on the board to which the signal wire of the servo is attached to.
        * @param feedback_pin Pin on the board the feedback wire of the servo is attached to.
        * @return the value return by the attach function from the Servo library.
        *   This value is the index number for the servo in the list of servos stored internaly by the Servo library.
        */
        uint8_t begin(int attach_pin, int feedback_pin);

        /*! 
        * Set the feedback pin that recieves the PWM position feedback.
        * @param pin The pin number on the bord.
        */
        uint8_t attachFeedbackPin(int pin);

        /*! 
        * Set the speed of the servo using a speed value in rotations per minute.
        * @param speed_rpm The speed in RPM. Should be between -140 and 140.
        * @return true if speed is set correctly, false if speed is out of range. If speed is out of range, it is set to 0.
        */
        bool setSpeed(int16_t speed_rpm);

        /*!
        * Set the speed of the servo using a speed value in rotations per minute.
        * Checks that the speed is within the range neccesary for feedback control to be possible.
        * @param speed_rpm The speed in RPM. Should be between -120 and 120.
        * @return true if speed is set correctly, false if speed is out of range. If speed is out of range, it is set to 0.
        */
        bool setSpeedWithFeedback(int16_t speed_rpm);

        /*! 
        * The writeMicroseconds function from the Servo class in the Arduino Servo library. 
        * Writes a value in microseconds (uS) to the servo, controlling the shaft accordingly. On a standard servo, this will set the angle of the shaft. 
        * On standard servos a parameter value of 1000 is fully counter-clockwise, 2000 is fully clockwise, and 1500 is in the middle.
        * Note that some manufactures do not follow this standard very closely so that servos often respond to values between 700 and 2300. 
        * Feel free to increase these endpoints until the servo no longer continues to increase its range. 
        * Note however that attempting to drive a servo past its endpoints (often indicated by a growling sound) is a high-current state, and should be avoided.
        * Continuous-rotation servos will respond to the writeMicrosecond function in an analogous manner to the write function.
        * @param value The value passed to the writeMicrosedonds function from the Servo class.
        */
        void writeMicroseconds(int value);

        /*! 
        * The readMicroseconds function from the Arduino Servo library.
        * Works by calling the readMicroseconds function from the Servo class.
        * @return the last value passed to write().
        */
        int readMicroseconds();

        /*! 
        * The detach function from the Arduino Servo library.
        * Works by calling the detach function from the Servo class.
        * @return the last value passed to write().
        */
        void detach();

        /*! 
        * The attached function from the Arduino Servo library.
        * Works by calling the attached function from the Servo class.
        * @return the last value passed to write().
        */
        bool attached();
        
        /*! 
        * The attach function from the Arduino Servo library.
        * Works by calling the attach function from the Servo class.
        * The overload attach(int pin, int min, int max) is intentionaly not included in the library.
        * @param pin The last value passed to write().
        * @return the value return by the attach function from the Servo library.
        *   This value is the index number for the servo in the list of servos stored internaly by the Servo library.
        */
        uint8_t attach(int pin);

        /*! 
        * Determin the angular position of the servo based on the PWM feedback.
        * Interupts are briefly turned off by this function for time critical code. 
        * For Arduino AVR and Teensy boards interupts are restored to what they were at the start of the function call.
        * For other boards, interupts are turned back on after the time critical code has run.
        * @return The curent angular position of the servo in degrees. 
        */
        double getAngularPosition();

        /*! 
        * Determin the speed of the servo in revolutinos per minute.
        */
        long getSpeedRPM();

        /*! 
        * Determin the speed of the servo in revolutinos per second.
        */
        double getSpeedRPS();

        /*! 
        * Determin the speed of the servo in deg per second.
        */
        double getSpeedDegPerSec();

        /*! 
        * Determin the speed of the servo in RPM.
        * The speed is a measurement based on how fast the measured angular position is changing.
        */
        float getMeasuredSpeedRPM();

        /*! 
        * Turn the servo motor from the currentn angular position to the desighred angular position.
        * If angle is greater than the starting angle, it turns clockwise.
        * If angle is less than the starting angle, it turns counterclockwise.
        * Interupts are disabled while parts of the function are running. 
        * Speed The absolute value of the speed in rpm to turn.
        * @param angle The angle to traval to.
        * @param speed_rpm The speed to turn at.
        * @return True if successfuly turned to desired angle.
        */
        bool turnTo(double angle, uint16_t speed_rpm);

        #ifdef PID_v1_h
        /*!
        * Use a PID controller to turn the servo to the desired angle.
        * Only available if the PID library, PID_v1.h, is included
        * @param angle The angle to turn to.
        */
        void turnToPID(double angle);
        #endif

        /*! 
        * Rotate the servo for a certain number of degrees.
        * Example: turnDeg(500) will rotate the motor through 500 deg. of rotation. 
        * @param deg The number of degrees to rotate the motor. Can be greater than 360. 
        * @param speed_rpm The speed at which the motor shoud turn.
        */
        void turn(double deg, int16_t speed_rpm);

        #ifdef PID_v1_h
        /*! 
        * Rotate the servo for a certain number of degrees using a PID controller.
        * Example: turnDeg(500) will rotate the motor through 500 deg. of rotation. 
        * @param deg The number of degrees to rotate the motor. Can be greater than 360. 
        */
        double turnPID(double deg);
        #endif

        /*! 
        * Turn the servo for a specified time.
        * @param sec The time to turn in seconds.
        * @param speed_rpm
        * @return the number of degrees that the motor should theoreticaly rotated.
        */
        double turnSec(uint32_t sec, int16_t speed_rpm);

        /*! 
        * Turn the servo for a specified time.
        * @param msec The time to turn in miliseconds.
        * @param speed_rpm
        * @return the number of degrees that the motor should theoreticaly rotated.
        */
        double turnMillis(uint32_t msec, int16_t speed_rpm);

        /*! 
        * Turn the servo motor from the currentn angular position to the desighred angular position.
        * Interupts are disabled while parts of the function are running. 
        * @param angle The angle to traval to.
        * @param speed_rpm The speed at which the motor shoud turn. 
        * @return True if motor turns to desired angle, False if it can not or invalid direction passed.
        */
        bool turnToShort(double angle, int16_t speed_rpm);
    };

}

#endif // FEEDBACK360SERVO_H