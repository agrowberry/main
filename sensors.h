class Sensors {
  public:
    #define SENS_1 12;
    #define SENS_2 18;
    #define SENS_3 20;
    #define SENS_4 21;
    #define SENS_5 22;
  
    #define NB_LS_PINS 5
  
    int sens_1_pin = 12;
    int sens_2_pin = 18;
    int sens_3_pin = 20;
    int sens_4_pin = 21;
    int sens_5_pin = 22;
  
    unsigned long sensor_read[5];
    

  int count_remaining(bool arr[]) {
    int len = 5;
    int rem = 0;
    int i;

    for (i = 0; i < len; i++) {
      if (arr[i] == false) {
        rem = rem + 1;
      }

    }
    return rem;
  }

  void pingAll() {

    int ls_pin[NB_LS_PINS] = {
      sens_1_pin,
      sens_2_pin,
      sens_3_pin,
      sens_4_pin,
      sens_5_pin
    };

    bool read_check[5] = {
      false,
      false,
      false,
      false,
      false
    };

    int which;
    int pin;
    int remaining;

    unsigned long start_time;
    unsigned long end_time;
    unsigned long elapsed_time;

    for (pin = 0; pin < 5; pin++) {

      pinMode(ls_pin[pin], OUTPUT);
      digitalWrite(ls_pin[pin], HIGH);

    }

    delayMicroseconds(10);

    for (pin = 0; pin < 5; pin++) {
      pinMode(ls_pin[pin], INPUT);
    }

    remaining = 5;

    start_time = micros();

    // While we still have sensors to read.
    while (remaining > 0) {

      // store microsecond count afterwards
      end_time = micros();

      elapsed_time = end_time - start_time;

      // iterate thru sensors and 
      for (which = 0; which < 5; which++) {

        if (digitalRead(ls_pin[which]) == LOW && read_check[which] == false) {

          sensor_read[which] = elapsed_time;
          read_check[which] = true;
          remaining = remaining - 1;
          //            Serial.print(which);
          //            Serial.print(" : ");
          //            Serial.print(remaining);
          //            Serial.print(" : ");
          //            Serial.print(elapsed_time);
          //            Serial.print("\n");

        }
      } // end of for

    } // end of while( remaining > 0 )
    Serial.println("Sensors Good");
  }

};
