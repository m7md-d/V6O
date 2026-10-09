#include "v6o.hpp"

#include <Arduino.h>

namespace v6o {

volatile bool running = false;


bool isRunning()
{ 
  return running; 
}

void pour(const PourStep &step)
{
  statusLed(true);

  const uint32_t start = millis();
  do {
    uint32_t elapsed = millis() - start;

    pump(elapsed % (step.pumpPulse.onMs + step.pumpPulse.offMs) < step.pumpPulse.onMs);

    for (int a = DOWN_ANGLE; a >= TILT_ANGLE; a--) {
      setPourAngle(a);
      plate(true);
    }

    for (int a = TILT_ANGLE; a <= DOWN_ANGLE; a++) {
      setPourAngle(a);
      plate(true);
    }
  } while (millis() - start < step.durationMs);

  pump(false);
  plate(false);
}

void run(const BrewProfile &profile)
{
  if (running)
    return;

  running = true;

  for (size_t i = 0; i < profile.pourCount; i++) {
    toggleLed(GREEN_LED, true);
    toggleLed(RED_LED, true);
    delay(profile.pours[i].waitBeforeMs);
    pour(profile.pours[i]);
  }
  statusLed(false);
  running = false;
}

void run()
{ 
  run(DEFAULT_PROFILE);
}

}
