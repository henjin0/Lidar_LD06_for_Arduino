#include <M5Atom.h>
#include "LD06forArduino.h"
#include <algorithm>
#include <math.h>

LD06forArduino ld06;
xSemaphoreHandle CtlSemaphore;

void draw_mat(const int way_type, int point) {
  switch (way_type) {
    case 0:
      M5.dis.drawpix(point - 1, CRGB::Green);
      return;
    case 1:
      M5.dis.drawpix(5 * (point) - 1, CRGB::Red);
      return;
    case 2:
      M5.dis.drawpix(25 - point, CRGB::White);
      return;
    case 3:
      M5.dis.drawpix(5 * (5 - point), CRGB::Blue);
      return;
  }
}

float convAngle0(float angle) {
  return angle >= 180 ? -(360 - angle) : angle;
}

void Task1(void *pvParameters) {
  M5.update();
  while (1) {
    //    Serial.printf("start_bytet:%d, data_length:%d, Speed:%f, FSA:%f, LSA:%f, time_stamp:%d, CS:%d",
    //                  ld06.start_byte, ld06.data_length, ld06.Speed, ld06.FSA, ld06.LSA, ld06.time_stamp, ld06.CS);
    //    Serial.println();

    float conv_angle;
    M5.dis.clear();
    for (int i = 0; i < ld06.data_length; i++) {
      if (((ld06.angles[i] > 360 - 45 && ld06.angles[i] <= 360) ||  ld06.angles[i] < 45 )
          && ld06.distances[i] < 100) {

        conv_angle = convAngle0(ld06.angles[i]);
        //Serial.println(conv_angle);
        draw_mat(0, round((conv_angle + 45) / 90.0 * 4) + 1);
      } else if (ld06.angles[i] >= 45 && ld06.angles[i] < 135 && ld06.distances[i] < 100) {
        conv_angle = ld06.angles[i];
        //Serial.println(conv_angle);
        draw_mat(1, round((conv_angle -45) / 90.0 * 4) + 1);
      }else if (ld06.angles[i] >= 135 && ld06.angles[i] < 225 && ld06.distances[i] < 100) {
        conv_angle = ld06.angles[i];
        //Serial.println(conv_angle);
        draw_mat(2, round((conv_angle - 135) / 90.0 * 4) + 1);
      }else if (ld06.angles[i] >= 225 && ld06.angles[i] < 315 && ld06.distances[i] < 100) {
        conv_angle = ld06.angles[i];
        //Serial.println(conv_angle);
        draw_mat(3, round((conv_angle - 225) / 90.0 * 4) + 1);
      }
    }
    delay(10);
  }
}

void setup() {
  M5.begin(true, false, true);
  delay(10);
  Serial.begin(115200);
  ld06.Init(22);

  vSemaphoreCreateBinary( CtlSemaphore );
  xTaskCreatePinnedToCore(
    Task1
    ,  "Task1"
    ,  4096
    ,  NULL
    ,  2 
    ,  NULL
    ,  0);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  //Serial.println("data start");
  ld06.read_lidar_data();

  //delay(10);
}
