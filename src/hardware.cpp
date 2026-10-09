#include "v6o.hpp"

namespace v6o {

Servo servo;
Stepper stepper(STEPS_PER_REVOLUTION, IN1, IN3, IN2, IN4);

void initHardware()
{
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  statusLed(false);

  servo.attach(SERVO_PIN);
  servo.write(DOWN_ANGLE);

  stepper.setSpeed(RPM);
  plate(false);
}

void pump(bool state)
{ 
  digitalWrite(PUMP_PIN, state ? HIGH : LOW);
}

void plate(bool state)
{
  if (state) {
    stepper.step(STEPS_PER_DEGREE);
    return;
  }

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setPourAngle(int angle)
{
  servo.write(angle);
}

void toggleLed(int led, bool state)
{ 
  digitalWrite(led, state ? HIGH : LOW); 
}

void statusLed(bool pouring)
{
  digitalWrite(RED_LED, pouring ? HIGH : LOW);
  digitalWrite(GREEN_LED, pouring ? LOW : HIGH);
}

bool buttonPressed()
{
  static bool wasDown = false;

  bool isDown = (digitalRead(BUTTON_PIN) == LOW);

  if (isDown == wasDown)
    return false;

  wasDown = isDown;


  return isDown;
}

}
