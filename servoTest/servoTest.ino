#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50

// サーボのチャンネル
#define SERVO_CHANNEL 0

// 初期位置（角度ではなくPWMカウント値）
#define SERVO_CENTER 350

void setServoPulse(int ch, int pulse) {
  pulse = constrain(pulse, 0, 4095);
  pwm.setPWM(ch, 0, pulse);
}

// すべてのサーボを初期位置に設定
void moveToInitialPosition() {
  setServoPulse(SERVO_CHANNEL, SERVO_CENTER);
}

void setup() {
  Wire.begin();

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  delay(500);

  moveToInitialPosition();
}

void loop() {
  // PCA9685がPWM信号を出し続けるため、再設定は不要
}
