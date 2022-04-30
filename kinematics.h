// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _KINEMATICS_H
#define _KINEMATICS_H

#include "encoders.h"


// Class to track robot position.
class Kinematics_c {
  public:
    // all distances in mm, and velocities in mm/s
    float width = 89.2;
    int velocity = 0;
    int dist_travelled = 0;
    float X = 0;
    float Y = 0;
    float delta_phi = 0;
    float phi = 0;
    float phi_deg = 0;
    float last_t = micros();
    float g_ratio = 1.53;
    
    // Constructor, must exist.
    Kinematics_c() {
      setupEncoder0();
      setupEncoder1();
    }
    
    // Use this function to update
    // your kinematics
    void update() {
      // Take time for velocity calcs
      unsigned long current_t = micros();
      unsigned long delta_t = current_t - last_t;
      // find distance travelled by individual wheels
      float l_distance = count_l * g_ratio;
      float r_distance = count_r * g_ratio;
      
      if (l_distance != r_distance) {
          delta_phi = (r_distance - l_distance) / width;
        } else {
          delta_phi = 0;
          }
      phi = phi + delta_phi;
      phi = fmod(phi, float(2*PI));
      phi_deg = (phi * (180/PI));
      float distance = (l_distance + r_distance) / 2;
      X = X + distance*cos(phi);
      Y = Y + distance*sin(phi);
      dist_travelled = dist_travelled + distance;

//      Serial.println(distance*cos(phi));
//      Serial.println(distance*sin(phi));
      count_l = 0;
      count_r = 0;
      
    }

    void zero() {
      velocity = 0;
      dist_travelled = 0;
      X = 0;
      Y = 0;
      delta_phi = 0;
      phi = 0;
    }

    void to_serial() {
        Serial.print(X);
        Serial.print(":");
        Serial.print(Y);
        Serial.print(":");
        Serial.print(phi * (180/PI));
        Serial.print("\n");
    }

      

};



#endif
