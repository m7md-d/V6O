#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Stepper.h>

#include <stdint.h>
#include <stddef.h>

namespace v6o {

constexpr size_t MAX_POURS = 10;

/**
 * A pattern of on and off pulses.
 */
struct PulsePattern {
    uint32_t onMs;
    uint32_t offMs;
};


constexpr PulsePattern DEFAULT_PUMP_PULSE = { .onMs = 1500, .offMs = 6000 };

/**
 * A single pour step in the brewing process.
 */
struct PourStep {
    uint32_t waitBeforeMs;
    uint32_t durationMs;
    PulsePattern pumpPulse;
};

/**
 * A complete brewing profile.
 */
struct BrewProfile {
    PourStep pours[MAX_POURS];
    size_t pourCount;
};


extern const BrewProfile DEFAULT_PROFILE;


// tube start at DOWN_ANGLE and tilts up to TILT_ANGLE
constexpr int DOWN_ANGLE = 180;
constexpr int TILT_ANGLE = 170;



constexpr int PUMP_PIN = 22;
constexpr int BUTTON_PIN = 23;
constexpr int GREEN_LED = 21;
constexpr int RED_LED = 4;

// servo
constexpr int SERVO_PIN = 16;

// plate stepper
constexpr int IN1 = 19;
constexpr int IN2 = 18;
constexpr int IN3 = 5;
constexpr int IN4 = 17;
constexpr int STEPS_PER_REVOLUTION = 2048;
constexpr int RPM = 10; // max 15
constexpr int STEPS_PER_DEGREE = 20;

// Hardware control functions.

void initHardware();
void pump(bool state);
void plate(bool state);
void setPourAngle(int angle);

void toggleLed(int led, bool state);
void statusLed(bool brewing);

bool buttonPressed();


void pour(const PourStep& step);

void run();
void run(const BrewProfile& profile);
bool isRunning();


bool connectWifi();
bool startApi();


void onButtonPressed();
void onApiRequest(const BrewProfile& profile);

}
