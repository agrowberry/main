#include "kinematics.h"


// create instance of kinematics class
Kinematics_c trace;


// motor class handling all commands to be given to 3pi+
class Motors {
  
      
      public:
        // define motor pins
        int l_dir_pin = 16;
        int r_dir_pin = 15;
        int l_pwm_pin = 10;
        int r_pwm_pin = 9;
        // arb. motor speed
        int motor_velocity;

    // method to set linear 3pi+ speed
    void set_velocity(int speed) {
          motor_velocity = speed;
          if (speed < 0){
            digitalWrite(l_dir_pin, 1);
            digitalWrite(r_dir_pin, 1);
          } else {
            digitalWrite(l_dir_pin, 0);
            digitalWrite(r_dir_pin, 0);
          }

          analogWrite(l_pwm_pin, abs(speed));
          analogWrite(r_pwm_pin, abs(speed));
      
      }

   // method to turn 3pi+ by deg degrees
   // 3pi+ is set to turn in a specified direction, and uses the kinematics trace class to measure its rotation.
   // When rotation reaches desired displacement (within tolerance of 1 degree) the turn is complete and the 3pi+ stops.
   void turn_degrees(float deg, bool left) {
          // set motor pins to correct sign for turn direction
          float degree_val;
          if (left) {
            degree_val = deg;
            digitalWrite(l_dir_pin, 1);
            digitalWrite(r_dir_pin, 0);              
          } else {
            degree_val = -deg;
            digitalWrite(l_dir_pin, 0);
            digitalWrite(r_dir_pin, 1);              
            }

          // create private kinematics class instance
          Kinematics_c turn_trace;

          // measure displacement
          turn_trace.update();

          // set motor speed
          analogWrite(l_pwm_pin, 20);
          analogWrite(r_pwm_pin, 20);

          // while loop checks for correct displacement
          while (abs(turn_trace.phi_deg - degree_val) > 1) {
            turn_trace.update();

            delay(10);
//            Serial.println(degree_val);
//            Serial.println(turn_trace.phi_deg - degree_val);
              
          }

          // return motor to linear motion previous to method call
          set_velocity(motor_velocity);
   }
    
  
};
