#include <Servo.h>
#include <SoftwareSerial.h>  

// 핀 정의
const int gas = A0;
const int dust = A1;
const int water = 2; //물감지센서는 습도가 아니라 디지털로
const int motor = 9; 

bool danger = false;

SoftwareSerial BT(10, 11); // HC-06 블루투스 통신

// 1. 가스 센서 함수
bool readGasSensor() {
  int gas = analogRead(gas);
  bool gaswarning = false;
  if(gas >= 300)
  {
    gaswarning = true;
  }
  return gaswarning;
}

// 2. 미세먼지 센서 함수
bool readDustSensor() {
  int raw = analogRead(dust);
  double voltage = raw * (5.0 / 1024.0);
  double dustDensity = (voltage - 0.1) * 0.5;
  double pm10 = dustDensity * 1000.0;
  if (pm10 < 0) pm10 = 0;
  bool dustwarning = false;
  if (pm10 >= 81 && pm10 <= 150) {
    dustwarning = true;
  }
  return dustwarning;
}

// 3. 물 감지 센서 함수
bool readWaterSensor() {
  int water = digitalRead(water); // 0: 물 있음
  return (water == 0); // 물이 감지되면 true 반환
}

// 4. 모터 회전 함수
void openDoor() {
  //for (int i = 0; i <; i++) {} 모터 크기랑 얼마나 도는지 테스트 해보고 설정
  
}

void closeDoor() {
  //for (int i = 0; i <; i++) {} 모터 크기랑 얼마나 도는지 테스트 해보고 설정
  
}

void openDoorRepeat(int count) {
  for (int i = 0; i < count; i++) {
    openDoor();
  }
}

void setup()
{
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(water, INPUT);
  pinMode(gas,INPUT);
  pinMode(dust,INPUT);
  pinMode(motor,OUTPUT);
}

void loop()
{
  if (BT.available()) {
    String command = BT.readStringUntil('\n');

    if (command == "OPEN") {
      openDoor();
    } else if (command == "CLOSE") {
      closeDoor();
    } else if (command.startsWith("REPEAT:")) {
      int count = command.substring(7).toInt();
      openDoorRepeat(count);
    }
  }

  bool dangerD = readDustSensor();
  bool dangerW = readWaterSensor();
  bool dangerG = readGasSensor();
  //위험 우선순위때문에 변수 3개 1순위 가스 2순위 물 3순위 미세먼지(2순위 물인이유 : 짜피 비오면 미세먼지 없음)
  if(dangerG == true)
  {
    openDoor(); //와이파이 모델 있었나? 사용자한테 알림 : 불난거같아요 or 가스누출인거 같아요 전송해야 되는데 블튜로는 10m 최대
  }
  else if(dangerG != true && dangerW == true)
  {
    closeDoor();
  }
  else if(dangerW != true && dangerD == true)
  {
    closeDoor();
  }

  delay(100000); // 100초마다 센서값 받음

  if(dangerG == false)
  {
    if(dangerW == true && dangerD == true)
    {
      closeDoor();
    }
  }
}

