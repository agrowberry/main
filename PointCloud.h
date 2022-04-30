

class PointCloud {
  public:
    int shape = 30;
    float points[30][3];
    int hits = 0;


  void add_point(float x, float y, float phi) {
    points[hits][0] = x;
    points[hits][1] = y;
    points[hits][2] = phi;
    hits += 1;
  }

  void zero_points() {
    int i;
    for (i=0; i<shape; i++) {
      points[i][0] = 0.0;
      points[i][1] = 0.0;
      points[i][2] = 0.0;
      
    }
  }

  void dump_to_serial() {
    int i; 
    for (i=0; i<shape; i++) {
        Serial.print(i);
        Serial.print(":");
        Serial.print(points[i][0]);
        Serial.print(":");
        Serial.print(points[i][1]);
        Serial.print(":");
        Serial.print(points[i][2]);
        Serial.print("\n");
    }
  }  
};
