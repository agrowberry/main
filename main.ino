#include "bumpers.h"

#include "motors.h"

#include "point_cloud.h"

#include "accel.h"


// create class instances

Bumpers Bump;
Motors Mot;
PointCloud pc_od;
PointCloud pc_ac;
IMU inertial;

// define actions for after detection of wall
void onBump() {
    pc_od.addPoint(trace.X, trace.Y, trace.phi);
    pc_ac.addPoint(trace.X, trace.Y, trace.phi);
    Mot.setVelocity(0);
    delay(1000);
    Mot.setVelocity(-20);
    delay(500);
    trace.update();
    Mot.turnDegrees(90, true);
    trace.phi += float(90 / (2 * PI));
    Bump.calibrateSensors();
    Mot.setVelocity(20);
    trace.update();
}



// define dump to serial of point cloud
void checkComplete() {
  // when point cloud reaches designated size sned data to serial
  while (pc_od.hits == pc_ac.shape) {
      Mot.setVelocity(0);
      Serial.println("Odometry Point Cloud Positional Data:");
      pc_od.dumpToSerial();
      Serial.println("Accelerometer Point Cloud Positional Data:");
      pc_ac.dumpToSerial();
      delay(1000);
  }
}




void setup() {
    Serial.begin(9600);
    Bump.calibrateSensors();
    Mot.setVelocity(20);
    pc_od.zeroPoints();
    pc_ac.zeroPoints();
    inertial.accelMeter();
    inertial.callibrateAccel();
}



void loop() {
  checkComplete();

  trace.update();
  inertial.readAccelGyro();
  
  // if bumper is hit
  if (Bump.bumpDetect(4500)) {
      onBump();
  } else if (trace.statDetect(10)) {
      onBump();
  }

}
