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
    float velocity = 0;
    int dist_travelled = 0;
    float X = 0;
    float Y = 0;
    float delta_phi = 0;
    float phi = 0;
    float phi_deg = 0;
    float last_t = micros();
    float g_ratio = 1.53;
    float l_distance = 0;
    float r_distance = 0;
    float last_l = 0;
    float last_r = 0;
    int count_same = 0;
    
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
      // store last distances for statDetect
      last_l = l_distance;
      last_r = r_distance;

      // find distance travelled by individual wheels
      l_distance = count_l * g_ratio;
      r_distance = count_r * g_ratio;
      
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
      velocity = distance / (delta_t / 1000000);

      count_l = 0;
      count_r = 0;
      last_t = micros();
    }

    // if wheels are stuck
    bool statDetect(int count_threshold) { 

      if ((last_l == l_distance) && (last_r == r_distance)) {
          count_same += 1;
      } else {
          count_same = 0;
          return false;
      }
      if (count_same >= count_threshold) {
          count_same = 0;
          return true;
      } 
    }
    

    void zero() {
      velocity = 0;
      dist_travelled = 0;
      X = 0;
      Y = 0;
      delta_phi = 0;
      phi = 0;
    }

    void toSerial() {
        Serial.print(X);
        Serial.print(":");
        Serial.print(Y);
        Serial.print(":");
        Serial.print(phi * (180/PI));
        Serial.print("\n");
    }

      

};



#endif
