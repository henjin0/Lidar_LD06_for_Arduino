#include <M5Atom.h>
#include <vector>
#include "AtomMotion.h"
#include <algorithm>

#define DATA_BYTE 36 //12 data *3 Byte
#define TOTAL_DATA_BYTE 48 //1(Start)+2(Datalen)+2(Speed)+2(SAngle)+36(DataByte)+2(EAngle)+2(TimeStamp)+1(CRC)

AtomMotion Atom;

void setup() {
  // put your setup code here, to run once:
  M5.begin(true, false, true);
  delay(10);
  Serial.begin(115200);
  Serial2.begin(230400, SERIAL_8N1, 19);
  Atom.Init();

}

void calc_lidar_data(std::vector<char> &values) {

  std::vector<float> angles;
  std::vector<float> distances;
  std::vector<int> confidences;

  char start_bit = values[0];
  char data_length = 0x1F & values[1];
  float Speed = float(values[3] << 8 | values[2]) / 100;
  float FSA = float(values[5] << 8 | values[4]) / 100;
  float LSA = float(values[values.size() - 4] << 8 | values[values.size() - 5]) / 100;
  int TimeStamp = int(values[values.size() - 2] << 8 | values[values.size() - 3]);
  int CS = int(values[values.size() - 1]);

  //Serial.printf("startbit:%d, data_length:%d, Speed:%f, FSA:%f, LSA:%f, TimeStamp:%d, CS:%d",
  //              start_bit, data_length, Speed, FSA, LSA, TimeStamp, CS);
  //Serial.println();

  float angleStep;
  if (LSA - FSA > 0) {
    angleStep = (LSA - FSA) / (data_length - 1);
  } else {
    angleStep = (LSA + (360 - FSA)) / (data_length - 1);
  }

  if(angleStep>20){
    //Serial.println("invalid data.");
    return;
  }
  

  for (int i = 0; i < data_length; i++) {
    float raw_deg = FSA + i * angleStep;
    angles.push_back(raw_deg <= 360 ? raw_deg : raw_deg - 360);
    confidences.push_back(values[8 + i * 3]);
    distances.push_back(float(values[8 + i * 3 - 1] << 8 | values[8 + i * 3 - 2]) / 100);
  
    if(angles[i] > -3 && angles[i]< 3 && distances[i]<3)
      Serial.print("Hit!");
  }

//  Serial.print("angles:");
//  std::for_each(angles.cbegin(), angles.cend(), [](float x) {
//    Serial.printf("%f,", x);
//  });
//  Serial.println();
//
//
//  Serial.print("distances:");
//  std::for_each(distances.cbegin(), distances.cend(), [](float x) {
//    Serial.printf("%f,", x);
//  });
//  Serial.println();
//
//
//  Serial.print("confidences:");
//  std::for_each(confidences.cbegin(), confidences.cend(), [](int x) {
//    Serial.printf("%d,", x);
//  });
//  Serial.println();


}

std::vector<char> tmpChars;

void get_lidar_data() {
  bool loopFlag = true;
  char tmpInt;



  while (loopFlag) {
    tmpInt = Serial2.read();

    if (tmpInt == 0x54 && tmpChars.empty()) {
      tmpChars.push_back(tmpInt);
      continue;
    } else if (tmpChars.size() == TOTAL_DATA_BYTE - 1 ) {
//      Serial.println();
//      std::for_each(tmpChars.cbegin(), tmpChars.cend(), [](char x) {
//        Serial.printf("%x,", x);
//      });
//      Serial.println();

      loopFlag = false;
      calc_lidar_data(tmpChars);
      tmpChars.clear();
      continue;
      
    } else if (tmpChars.size() > 0) {
      if (tmpChars[0] == 0x54) {
        tmpChars.push_back(tmpInt);
      }
      if (tmpChars.size() > 1) {
        if (tmpChars[1] != 0x2C) {
          tmpChars.clear();
        }
      }
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  if (Serial2.available()) {

    //Serial.println("data start");
    get_lidar_data();

    //    // 受信データを取得
    //    char ch = Serial2.read();
    //    // 受信データをラズパイ側へそのまま送信
    //    Serial.print(ch);
  }

  //delay(10);
}
