# Lidar_LD06_for_Arduino
This code can use Lidar LD06 provided by LDROBOT from arduino.

LDROBOT社から提供されているLD06をArduinoから使用できるようにするプログラムです。

# Attention
LD06 uses UART communication. In this program, Serial2 is used to receive data.
If you want to use Serial2, please replace Serial2 in LD06forArduino.ino with another Serial object.

LD06との通信はUART通信です。本プログラムではSerial2を使用してデータを受信しています。Serial2を使用したい場合はLD06forArduino.inoのSerial2を任意のSerialオブジェクトに差し替えてください。

# How to install
Place LD06forArduino.h and LD06forArduino.ino in the same folder as the ino file you want to use.
After that, please include LD06forArduino.h with the ino file.

LD06forArduino.hとLD06forArduino.inoを使用するinoファイルと同じフォルダに配置してください。使う際にはLD06forArduino.hをinoファイルでインクルードしてください。

# How to use

## void Init(const int pin)
This is the initialization function. Execute it first.
Specify the Rx pin for receiving data from the LD06 as argument.

本プログラムを使用する際には必ず最初に実行してください。
LD06からデータを受信するためのRxピンを指定します。

```
// example
ld06.Init(19);//use pin19
```

## void read_lidar_data()
Load the data acquired by LD06 into an object. The loaded data can be referenced from the object's member variables.

LD06が取得したデータをオブジェクトに読み込みます。読み込んだデータはオブジェクトのメンバー変数から参照できます。

```
// example
ld06.read_lidar_data();
```

| Member | Type | Detail |
| ------------- | ------------- | ------------- |
| start_byte | char | Communication start byte0x54)  |
| data_length | char | Acquisition data num(By the specification of LD06, it is fixed to 12.)  |
| Speed | float | Lidar rotation velocity　(deg/s) |
| FSA | float | Data start angle(deg) |
| LSA | float | Data end angle(deg) |
| time_stamp | int | timestamp of when Acquisition data(ms). MAX:30000 |
| CS | int | CRCcheck value  |
| angle_step | int | Angle ticks per data(deg)  |
| angles | std::vector<float> | Angle of acquired data(deg) |
| distances | std::vector<float> | Distance of acquired data(mm) |
| confidences | std::vector<int> | Strength of light reflection |

| メンバ変数 | 型 | 詳細 |
| ------------- | ------------- | ------------- |
| start_byte | char | 通信開始を表す１バイト(0x54)  |
| data_length | char | 取得したデータ数(LD06の仕様につき12個固定)  |
| Speed | float | Lidarの回転速度　(deg/s) |
| FSA | float | データの開始角度(deg) |
| LSA | float | データの終端角度(deg) |
| time_stamp | int | データ取得時のタイムスタンプ(ms) 30000が最大値 |
| CS | int | CRCcheckの値  |
| angle_step | int | 位置データ毎の刻み角度(deg)  |
| angles | std::vector<float> | 取得したデータの角度(deg) |
| distances | std::vector<float> | anglesの要素に対応した距離(mm) |
| confidences | std::vector<int> | 光の反射の強さ  |

Please refer to LD06_sample.ino for how to use it. (It has been tested with Atom lite provided by M5Stack.) 
  
使用方法などはLD06_sample.inoを参照してください。（M5Stack社が提供するAtom liteで動作確認済みです。）
  
# LD06(LDS06)について
- LD06(LDS06)入手先 https://www.inno-maker.com/product/lidar-ld06/
- (データシート、SDKなど) http://wiki.inno-maker.com/display/HOMEPAGE/LD06

# ライセンス
Please see [LICENSE](https://github.com/henjin0/Lidar_LD06_for_Arduino/blob/main/LICENSE).
