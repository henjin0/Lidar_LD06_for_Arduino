#include <M5Atom.h>
#include "LD06forArduino.h"
#include <algorithm>

LD06forArduino ld06;

void setup() {
  M5.begin(true, false, true);
  delay(10);
  Serial.begin(115200);
  ld06.Init(19);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  //Serial.println("data start");
  ld06.read_lidar_data();

  Serial.printf("startbit:%d, data_length:%d, Speed:%f, FSA:%f, LSA:%f, TimeStamp:%d, CS:%d",
                ld06.start_bit, ld06.data_length, ld06.Speed, ld06.FSA, ld06.LSA, ld06.TimeStamp, ld06.CS);
  Serial.println();

  for (int i = 0; i < ld06.data_length; i++) {
    if (ld06.angles[i] > -3 && ld06.angles[i] < 3 && ld06.distances[i] < 3)
      Serial.print("Hit!");
  }


  //delay(10);
}
