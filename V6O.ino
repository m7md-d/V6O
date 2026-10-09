#include "src/v6o.hpp"

void setup() {
  Serial.begin(115200);

  v6o::initHardware();

  if (v6o::connectWifi())
    v6o::startApi();
  else
    Serial.println("No network");
}

void loop() {
  if (v6o::buttonPressed())
    v6o::onButtonPressed();

  delay(10);
}
