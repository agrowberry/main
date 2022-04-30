class Bumpers {
  public:
    int lb_pin = 4;
  int rb_pin = 5;
  unsigned long l_norm;
  unsigned long r_norm;
  unsigned long sensor_read[2];

  void pingAll() {
    int pins[2] = {
      lb_pin,
      rb_pin
    };

    bool read_check[2] = {
      false,
      false
    };

    int which;
    int pin;
    int remaining;

    unsigned long start_time;
    unsigned long end_time;
    unsigned long elapsed_time;

    for (pin = 0; pin < 2; pin++) {

      pinMode(pins[pin], OUTPUT);
      digitalWrite(pins[pin], HIGH);

    }

    delayMicroseconds(10);

    for (pin = 0; pin < 2; pin++) {
      pinMode(pins[pin], INPUT);
    }

    remaining = 2;
    start_time = micros();

    // While we still have sensors to read.
    while (remaining > 0) {

      // store microsecond count afterwards
      end_time = micros();

      elapsed_time = end_time - start_time;

      // iterate through sensors and find which have fallen low
      for (which = 0; which < 2; which++) {

        if (digitalRead(pins[which]) == LOW && read_check[which] == false) {

          sensor_read[which] = elapsed_time;
          read_check[which] = true;
          remaining = remaining - 1;

        }
      } // end of for

    } // end of while( remaining > 0 )

  }

  // calibrate un-hit bump sensor, if value is fluctuating > 10% then keep checking
  void calibrate_sensors() {

    pingAll();
    l_norm = sensor_read[0];
    r_norm = sensor_read[1];
    delayMicroseconds(1000);

    while ((abs((sensor_read[0] - l_norm) / sensor_read[0]) > 0.1) || (abs((sensor_read[0] - l_norm) / sensor_read[0]) > 0.1)) {
      pingAll();
      l_norm = sensor_read[0];
      r_norm = sensor_read[1];
      delayMicroseconds(1000);
    }
  }

  bool detect_hit(int sensitivity) {
    pingAll();
    if ((sensor_read[0] > sensitivity + l_norm) || (sensor_read[1] > sensitivity + r_norm)) {
      return true;
    } else {
      calibrate_sensors();
      return false;
    }

  }

  // print raw sensor values to serial
  void to_serial() {

    Serial.print("LB: ");
    Serial.print(sensor_read[0]);
    Serial.print(" - RB: ");
    Serial.print(sensor_read[1]);
    Serial.print("\n");

  }

};
