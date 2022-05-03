#include <Wire.h>
#include <LSM6.h>

class IMU{
  public:
    LSM6 imu;
    
   long vel_x = 0;
   long vel_y = 0;
   long dis_x = 0;
   long dis_y = 0;
   long delta_vel_x = 0;
   long delta_vel_y = 0;
   long delta_dis_x = 0;
   long delta_dis_y = 0;
   long ave_dis = 0;
   
   float acc_x, acc_y, acc_z;
   float gyro_x, gyro_y, gyro_z;
   float acc_ang_x, acc_ang_y, gy_ang_x, gy_ang_y, gy_ang_z;
   float roll, pitch, yaw;
   float acc_error_x, acc_error_y, gy_error_x, gy_error_y, gy_error_z;
   float elapsed_time, current_time, previous_time;
   int c = 0;

   long prev_acc_x = 0;
   long prev_acc_y = 0;

      
 
 unsigned long start_time;

void accelMeter(){
  
  Wire.begin();

  // Serial for debug output
  Serial.begin(9600);
  Serial.println("***RESET***");
  delay(1000);

  // Check the IMU initialised ok.
  if (!imu.init() ) {  // no..? :(

    while (1) {
      Serial.println("Failed to detect and initialize IMU!");
      delay(1000);
    }
  }

  // IMU initialise ok!
  // Set the IMU with default settings.
  imu.enableDefault();
  
  setupEncoder0();
  setupEncoder1();
  
}

float callibrateAccel(){
  
  imu.read();
  
  imu.writeReg(LSM6::CTRL1_XL, 0b01001000);
  
  imu.a.x = imu.a.x*0.244;
  imu.a.y = imu.a.y*0.244;
  imu.a.z = imu.a.z*0.244;
  float abaccel = sqrt((imu.a.x)^2+(imu.a.y)^2+(imu.a.z)^2);

  imu.g.x = imu.g.x/8.75;
  imu.g.y = imu.g.y/8.75;
  imu.g.z = imu.g.z/8.75;
  float abgyro = sqrt((imu.g.x)^2+(imu.g.y)^2+(imu.g.z)^2);

  unsigned long current_time = millis();
//  unsigned long delta_t;
  if ( isnan(abgyro) ){
    abgyro = 0;
  }
  if ( isnan(abaccel) ){
    abaccel = 0;
  }
}

void readAccelGyro(){

  imu.read();
  
  imu.writeReg(LSM6::CTRL1_XL, 0b01001000);
  
  acc_x = imu.a.x*0.244;
  acc_y = imu.a.y*0.244;

  // Calculating Roll and Pitch from the accelerometer data
  acc_ang_x = (atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) 
  acc_ang_y = (atan(-1 * acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
          
  current_time = micros();            
  elapsed_time = (current_time - previous_time)/100000; 
 
  long current_acc_x = acc_x;
  long current_acc_y = acc_y;

//  if ( abs((current_acc_x - prev_acc_x)/prev_acc_x) < 0.005) {
//      Serial.println("No Change");
//      Serial.println((current_acc_x - prev_acc_x)/prev_acc_x);
//      delta_dis_x = 0;
//  }
//
//  if ( abs((current_acc_y - prev_acc_y)/prev_acc_y) < 0.005) {
//      delta_dis_y = 0;
//      
//  } else {
      if (abs(acc_x) < 100) {
        acc_x = 0;
        vel_x = 0.9 * vel_x;
      }

      if (abs(acc_y) < 100) {
        acc_y = 0;
        vel_y = 0.9 * vel_y;
      }
      delta_vel_y = acc_y * elapsed_time ;
      vel_y = vel_y + delta_vel_y ;
      delta_vel_x = acc_x * elapsed_time ;
      vel_x = vel_x + delta_vel_x ;
      
      delta_dis_y = vel_y * elapsed_time ;
      dis_y += delta_dis_y;
      delta_dis_x = vel_x * elapsed_time ;
      dis_x += delta_dis_x; 
//  }
     
  gyro_x =  imu.g.x/8.75/ 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0
  gyro_y =  imu.g.y/8.75/ 131.0;
  gyro_z =  imu.g.z/8.75/ 131.0;
  
  gyro_x = gyro_x + 0.56; // GyroErrorX ~(-0.56)
  gyro_y = gyro_y - 2; // GyroErrorY ~(2)
  gyro_z = gyro_z + 0.79; // GyroErrorZ ~ (-0.8)
  
  gy_ang_x = gy_ang_x + gyro_x * elapsed_time; // deg/s * s = deg
  gy_ang_y = gy_ang_y + gyro_y * elapsed_time;
  
  yaw =  yaw + gyro_z * elapsed_time;
  roll =  0.09 * acc_ang_x;
  pitch = 0.09 * acc_ang_y;

  long change_dis = sqrt(((dis_x)*(dis_x))+((dis_y)*(dis_y)));

  ave_dis = ave_dis + (change_dis);

  calculateImuError();

  prev_acc_x = acc_x;
  prev_acc_y = acc_y;
  previous_time = micros();
}

void calculateImuError() {

  // Read accelerometer values 200 times
  while (c < 200) {
  
    acc_x = imu.a.x*0.244 ;
    acc_y = imu.a.y*0.244 ;
    acc_z = imu.a.z*0.244 ;
    // Sum all readings
    acc_error_x = acc_error_x + ((atan((acc_x) / sqrt(pow((acc_y), 2) + pow((acc_z), 2))) * 180 / PI));
    acc_error_y = acc_error_y + ((atan(-1 * (acc_x) / sqrt(pow((acc_y), 2) + pow((acc_y), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  acc_error_x = acc_error_x/ 200;
  acc_error_y= acc_error_y / 200;
  c = 0;
  
  // Read gyro values 200 times
  while (c < 200) {

    gyro_x = imu.g.x/8.75;
    gyro_y = imu.g.x/8.75;
    gyro_z = imu.g.x/8.75;
    // Sum all readings
    gy_error_x = gy_error_x + (gyro_x / 131.0);
    gy_error_y = gy_error_y + (gyro_y / 131.0);
    gy_error_z = gy_error_z + (gyro_z / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  gy_error_x = gy_error_x / 200;
  gy_error_y = gy_error_y / 200;
  gy_error_z = gy_error_z / 200;
}

void displacementToSerial() {
  Serial.print("Displacement - X:");
  Serial.print(dis_x);
  Serial.print(" Y:");
  Serial.print(dis_y);
  Serial.print("\n");
}

void velocityToSerial() {
  Serial.print("Velocity - X:");
  Serial.print(vel_x);
  Serial.print(" Y:");
  Serial.print(vel_y);
  Serial.print("\n");
}

void accelerationToSerial() {
  Serial.print("Acceleration - X:");
  Serial.print(acc_x);
  Serial.print(" Y:");
  Serial.print(acc_y);
  Serial.print("\n");
}

};
