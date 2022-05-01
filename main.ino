#include "bumpers.h"

#include "motors.h"

#include "point_cloud.h"

#include "accel.h"


// create class instances
Sensors Sens;
Bumpers Bump;
Motors Mot;
PointCloud pc_od;
PointCloud pc_ac;
IMU inertial;

// define actions for after detection of wall
void onBump() {
  pc_od.add_point(trace.X, trace.Y, trace.phi);
  pc_ac.add_point(trace.X, trace.Y, trace.phi);
  Mot.set_velocity(0);
  delay(1000);
  Mot.set_velocity(-20);
  delay(500);
  trace.update();
  Mot.turn_degrees(90, true);
  trace.phi += float(90 / (2 * PI));
  Mot.set_velocity(20);
  trace.update();
}

float last_x;
float last_y;
int count_same = 0;
int but_A = 14;

void setup() {
  Serial.begin(9600);
  Bump.calibrate_sensors();
  Mot.set_velocity(20);
  pc_od.zero_points();
  pc_ac.zero_points();
  inertial.AccelMeter();
  inertial.CallibrateAccel();
}

void loop() {
  // when point cloud reaches designated size sned data to serial
  while (pc_od.hits == pc_ac.shape) {
    Mot.set_velocity(0);
    Serial.println("Odometry Point Cloud Positional Data:");
    pc_od.dump_to_serial();
    Serial.println("Accelerometer Point Cloud Positional Data:");
    pc_ac.dump_to_serial();
    delay(1000);
  }

  trace.update();
  inertial.Read_Accel_Gyro();

  // if bumper is hit
  if (Bump.detect_hit(7500)) {
    onBump();
  }

  // if wheels are stuck
  // measures if 
  if ((trace.X == last_x) && (trace.Y == last_y)) {
    count_same += 1;
    if (count_same > 10) {
      onBump();
      count_same = 0;
    }
  } else {
    count_same = 0;
  }

  last_x = trace.X;
  last_y = trace.Y;
}
