#include <Servo.h>

// >1500 anti-clockwise
// <1500 clockwise
Servo servoUp;  // Servo motor controlling upward movement
Servo servoDown;  // Servo motor controlling downward movement

const int upLedPin = 10;  // Pin for the upper IR LED
const int upReceiverPin = 11;  // Pin for the upper IR receiver
const int downLedPin = 2;  // Pin for the lower IR LED
const int downReceiverPin = 3;  // Pin for the lower IR receiver
const int midLedPin = 6;  // Pin for the middle IR LED
const int midReceiverPin = 7;  // Pin for the middle IR receiver

int upSpeed = 1570;  // Initial speed for the upper servo
int downSpeed = 1430;  // Initial speed for the lower servo
boolean isEqual = false;  // Turn error compensation flag
int lastMid = 0;  // Variable to store the last middle IR sensor reading
boolean isBackTurn = false;  // Flag indicating if the robot is returning
int countTurn = 0;  // Counter for the number of turns
int countMove = 0;  // Counter for the number of movements
boolean isLastLine = false;  // Flag indicating if the robot is at the last line

void setup() {
   pinMode(upReceiverPin, INPUT);  // Set the upper IR receiver pin as input
   pinMode(upLedPin, OUTPUT);  // Set the upper IR LED pin as output
   pinMode(downReceiverPin, INPUT);  // Set the lower IR receiver pin as input
   pinMode(downLedPin, OUTPUT);  // Set the lower IR LED pin as output
   pinMode(midReceiverPin, INPUT);  // Set the middle IR receiver pin as input
   pinMode(midLedPin, OUTPUT);  // Set the middle IR LED pin as output
   servoUp.attach(13);  // Attach the upper servo motor to pin 13
   servoDown.attach(12);  // Attach the lower servo motor to pin 12
   Serial.begin(9600);  // Start serial communication at 9600 baud rate
}

void loop() {
  int upResult = irDistance(upLedPin, upReceiverPin);  // Read the upper IR sensor
  int downResult = irDistance(downLedPin, downReceiverPin);  // Read the lower IR sensor
  int midResult = irDistance(midLedPin, midReceiverPin);  // Read the middle IR sensor

  if (isEqual) {  // Error compensation when sensor values are equal
    turnRight(400);  // Turn right to adjust
    upResult = irDistance(upLedPin, upReceiverPin);  // Re-read sensor values
    downResult = irDistance(downLedPin, downReceiverPin);
    midResult = irDistance(midLedPin, midReceiverPin);
    
    if (lastMid > midResult) {  // If midResult decreased, adjust further
      moveBack(400);  // Move back to correct position
      turnLeft(900);  // Turn left to compensate
    } else {
      turnRight(600);  // Otherwise, turn right for further adjustment
    }
    isEqual = false;  // Reset the error compensation flag
    upResult = irDistance(upLedPin, upReceiverPin);  // Re-check sensor values
    downResult = irDistance(downLedPin, downReceiverPin);
    midResult = irDistance(midLedPin, midReceiverPin);
  }

  if (midResult > 4) {  // If the robot is not on a line (or in a straight path)
    if (isLastLine) {
      countMove++;
      if (countMove == 30) {  
        // Stop before the red line
        servoUp.writeMicroseconds(1500);  // Stop the upper servo
        servoDown.writeMicroseconds(1500);  // Stop the lower servo
        delay(8000);  // Wait for 8 seconds
      }
    }

    if (upResult >= 2 && downResult > 2) {
      Serial.println("move forward");
      upSpeed = 1570;  // Set speed for forward movement
      downSpeed = 1430;
      move(upSpeed, downSpeed);  // Move the robot forward
    }
    else if (upResult < 2 && downResult > 4) {
      Serial.println("adjust left");
      upSpeed = 1650;  // Adjust speed for left turn
      downSpeed = 1440;
      move(upSpeed, downSpeed);  // Move the robot to the left
    }
    else if (upResult >= 4 && downResult < 3) {
      Serial.println("adjust right");
      upSpeed = 1550;  // Adjust speed for right turn
      downSpeed = 1400;
      move(upSpeed, downSpeed);  // Move the robot to the right
    }
  } else {
    // Turn logic when the robot encounters a line (or turn point)
    if (countTurn == 10 && !isBackTurn) {
        Serial.println("10 turns!");
        move(upSpeed, downSpeed);  // Move a little bit forward
        servoUp.writeMicroseconds(1460);  // Turn the servos for left turn
        servoDown.writeMicroseconds(1450);
        delay(1100);
        servoUp.writeMicroseconds(1440);  // Turn to move back
        servoDown.writeMicroseconds(1553);
        delay(2000);
        isBackTurn = true;  // Set back-turn flag
        countTurn = 0;  // Reset turn count
    } else {
        countTurn++;
        if (countTurn == 11 && isBackTurn) {
          // The last straight line
          Serial.println("11 turns!");
          isLastLine = true;
          countMove = 0;
        }

        // Turn based on IR sensor readings
        if (upResult == 5 && upResult > downResult) {
            Serial.println("turn left");
            turnLeft(1000);  // Turn left for correction
        }
        else if (downResult == 5 && downResult > upResult) {
            Serial.println("turn right");
            turnRight(1000);  // Turn right for correction
        }
        else if (upResult > downResult) {
            Serial.println("turn left");
            turnLeft(1000);  // Turn left
        }
        else if (downResult > upResult) {
          Serial.println("turn right");
          turnRight(1000);  // Turn right
        }
        else {
          // If the sensor values are equal
          Serial.println("equal (right)");
          isEqual = true;  // Set the error compensation flag
          lastMid = midResult;  // Store the current middle sensor value
        }
    }
  }
}

// Move the robot forward or backward
void move(int up, int down) {
  servoUp.writeMicroseconds(up);  // Control the upper servo
  servoDown.writeMicroseconds(down);  // Control the lower servo
  delay(100);  // Short delay for stability
}

// Measure the distance using IR sensors
int irDistance(int LedPin, int ReceiverPin) {
  int distance = 0;
  for (long f = 38000; f <= 42000; f += 1000) {
    distance += irDetect(LedPin, ReceiverPin, f);  // Detect IR signal at different frequencies
  }
  return distance;
}

// Detect IR signal at a specific frequency
int irDetect(int LedPin, int ReceiverPin, long frequency) {
  tone(LedPin, frequency);  // Emit IR signal at the given frequency
  delay(1);  // Short delay
  noTone(LedPin);  // Stop emitting the signal
  int ir = digitalRead(ReceiverPin);  // Read the IR receiver
  delay(1);  // Short delay
  return ir;  // Return the detection result
}

// Move the robot backward for a given time
void moveBack(int time) {
  servoUp.writeMicroseconds(1260);  // Set speed for backward movement
  servoDown.writeMicroseconds(1500);  // Set the reverse speed for the lower servo
  delay(time);  // Wait for the specified time
}

// Turn the robot right for a specified time
void turnRight(int time) {
  servoUp.writeMicroseconds(1800);  // Set speed for right turn
  servoDown.writeMicroseconds(1500);  // Keep the lower servo fixed
  delay(time);  // Wait for the specified time
}

// Turn the robot left for a specified time
void turnLeft(int time) {
  servoUp.writeMicroseconds(1500);  // Keep the upper servo fixed
  servoDown.writeMicroseconds(1260);  // Set speed for left turn
  delay(time);  // Wait for the specified time
}






//한국어

#include <Servo.h>

// >1500: 시계 반대 방향 회전 (anti-clockwise)
// <1500: 시계 방향 회전 (clockwise)
Servo servoUp;
Servo servoDown;
const int upLedPin = 10;  // 위쪽 LED 핀
const int upReceiverPin = 11;  // 위쪽 수신기 핀
const int downLedPin = 2;  // 아래쪽 LED 핀
const int downReceiverPin = 3;  // 아래쪽 수신기 핀
const int midLedPin = 6;  // 중앙 LED 핀
const int midReceiverPin = 7;  // 중앙 수신기 핀

int upSpeed = 1570;  // 위쪽 서보 초기 속도 (microseconds)
int downSpeed = 1430;  // 아래쪽 서보 초기 속도 (microseconds)
boolean isEqual = false;  // 회전 오류 보정
int lastMid = 0;  // 마지막 중앙 센서 값
boolean isBackTurn = false;  // 회전 후 돌아오기 여부
int countTurn = 0;  // 회전 횟수
int countMove = 0;  // 이동 횟수
boolean isLastLine = false;  // 마지막 선인지 여부

void setup()                                
{
   pinMode(upReceiverPin, INPUT);  // 위쪽 수신기 핀 입력 모드 설정            
   pinMode(upLedPin, OUTPUT);  // 위쪽 LED 핀 출력 모드 설정  
   pinMode(downReceiverPin, INPUT);  // 아래쪽 수신기 핀 입력 모드 설정            
   pinMode(downLedPin, OUTPUT);  // 아래쪽 LED 핀 출력 모드 설정
   pinMode(midReceiverPin, INPUT);  // 중앙 수신기 핀 입력 모드 설정            
   pinMode(midLedPin, OUTPUT);  // 중앙 LED 핀 출력 모드 설정
   servoUp.attach(13);  // 서보 모터 위쪽 연결 (핀 13)
   servoDown.attach(12);  // 서보 모터 아래쪽 연결 (핀 12)
   Serial.begin(9600);  // 시리얼 통신 시작 (9600 보드레이트)
}  

void loop()                               
{
  // 각 방향의 거리 값을 읽어옴 (IR 센서로 미로의 벽과 거리를 측정)
  int upResult = irDistance(upLedPin, upReceiverPin);
  int downResult = irDistance(downLedPin, downReceiverPin);
  int midResult = irDistance(midLedPin, midReceiverPin);

  if (isEqual) {  // 회전 오류 보정 처리
    turnRight(400);  // 오류가 있을 때 오른쪽으로 회전
    upResult = irDistance(upLedPin, upReceiverPin);
    downResult = irDistance(downLedPin, downReceiverPin);
    midResult = irDistance(midLedPin, midReceiverPin);

    if (lastMid > midResult) {  // 센서 값이 더 커졌다면 보정
      moveBack(400);  // 뒤로 이동
      turnLeft(900);  // 왼쪽으로 회전
    }
    else { 
      turnRight(600);  // 오른쪽으로 회전
    }
    isEqual = false;  // 보정 상태를 초기화
    upResult = irDistance(upLedPin, upReceiverPin);
    downResult = irDistance(downLedPin, downReceiverPin);
    midResult = irDistance(midLedPin, midReceiverPin);
  }

  if (midResult > 4) {  // 중앙 센서가 4 이상이면 직진 또는 미세 조정
    if (isLastLine) {
      countMove++;  // 마지막 선에 도달했으면 카운트
      if (countMove == 30) {  // 30번 이동 후 정지
        servoUp.writeMicroseconds(1500);  // 서보 정지
        servoDown.writeMicroseconds(1500);  // 서보 정지
        delay(8000);  // 8초 대기
      }
    }
    
    // 위쪽과 아래쪽 센서 값에 따라 방향 조정
    if (upResult >= 2 && downResult > 2) {
      Serial.println("move forward");  // 직진
      upSpeed = 1570;
      downSpeed = 1430;
      move(upSpeed, downSpeed);
    }
    else if (upResult < 2 && downResult > 4) {
      Serial.println("adjust left");  // 왼쪽으로 조정
      upSpeed = 1650;
      downSpeed = 1440;
      move(upSpeed, downSpeed);
    }
    else if (upResult >= 4 && downResult < 3) {
      Serial.println("adjust right");  // 오른쪽으로 조정
      upSpeed = 1550;
      downSpeed = 1400;
      move(upSpeed, downSpeed);
    }
  }
  else {  // 센서 값이 4 이하일 경우 회전 처리
    if (countTurn == 10 && isBackTurn == false) {  // 10회 회전 후 720도 회전
        Serial.println("10 turns!");  // 10회전 후 회전 시작
        move(upSpeed, downSpeed);  // 조금 더 전진
        servoUp.writeMicroseconds(1460);  // 서보 조정
        servoDown.writeMicroseconds(1450);
        delay(1100);  // 1.1초 대기
        servoUp.writeMicroseconds(1440);  // 서보 초기화
        servoDown.writeMicroseconds(1553);
        delay(2000);  // 2초 대기
        isBackTurn = true;  // 회전 후 돌아오기 상태로 변경
        countTurn == 0;
      }
    else {
        countTurn++;  // 회전 횟수 증가
        if (countTurn == 11 && isBackTurn) {
          Serial.println("11 turns!");  // 마지막 직선
          isLastLine = true;  // 마지막 선 설정
          countMove = 0;  // 이동 횟수 초기화
        }

        // 위쪽과 아래쪽 센서 값에 따라 회전
        if (upResult == 5 && upResult > downResult) {
            Serial.println("turn left");  // 왼쪽으로 회전
            turnLeft(1000);
        }
        else if (downResult == 5 && downResult > upResult) {  
            Serial.println("turn right");  // 오른쪽으로 회전
            turnRight(1000);
        }
        else if (upResult > downResult) {
            Serial.println("turn left");  // 왼쪽으로 회전
            turnLeft(1000);
        }
        else if (downResult > upResult) {
          Serial.println("turn right");  // 오른쪽으로 회전
          turnRight(1000);
        }
        else {
          // 센서 값이 같을 경우 보정
          Serial.println("equal (right)");  
          isEqual = true;  // 보정 상태 설정
          lastMid = midResult;  // 마지막 중앙 센서 값 저장
        }
    }
  }
}

// 서보 이동 함수
void move(int up, int down) {
  servoUp.writeMicroseconds(up);  // 위쪽 서보 이동
  servoDown.writeMicroseconds(down);  // 아래쪽 서보 이동
  delay(100);  // 100ms 대기
}

// IR 거리 측정 함수
int irDistance(int LedPin, int ReceiverPin) {
  int distance = 0;
  for (long f = 38000; f <= 42000; f += 1000) {
    distance += irDetect(LedPin, ReceiverPin, f);  // 각 주파수로 거리 측정
  }
  return distance;  // 측정된 거리 반환
}

// IR 신호 감지 함수
int irDetect(int LedPin, int ReceiverPin, long frequency)
{
  tone(LedPin, frequency);  // 주파수 신호 출력
  delay(1);  // 잠시 대기
  noTone(LedPin);  // 신호 끄기
  int ir = digitalRead(ReceiverPin);  // 수신기에서 신호 읽기
  delay(1);  // 잠시 대기
  return ir;  // 신호 반환
}

// 뒤로 이동 함수
void moveBack(int time) {
  servoUp.writeMicroseconds(1260);  // 위쪽 서보 뒤로 설정
  servoDown.writeMicroseconds(1500);  // 아래쪽 서보 정지
  delay(time);  // 지정된 시간만큼 대기
}

// 오른쪽으로 회전 함수
void turnRight(int time) {
  servoUp.writeMicroseconds(1800);  // 위쪽 서보 오른쪽으로 회전
  servoDown.writeMicroseconds(1500);  // 아래쪽 서보 정지
  delay(time);  // 지정된 시간만큼 대기
}

// 왼쪽으로 회전 함수
void turnLeft(int time) {
  servoUp.writeMicroseconds(1500);  // 위쪽 서보 정지
  servoDown.writeMicroseconds(1260);  // 아래쪽 서보 왼쪽으로 회전
  delay(time);  // 지정된 시간만큼 대기
}