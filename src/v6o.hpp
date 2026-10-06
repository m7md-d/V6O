#pragma once

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

/**
 * A single pour step in the brewing process.
 */
struct PourStep {
    uint32_t durationMs;
    uint32_t restAfterMs;

    PulsePattern pump;
    PulsePattern plate;

    float angleStart;
    float angleEnd;
};

/**
 * A complete brewing profile.
 */
struct BrewProfile {
    uint32_t initialDelayMs;

    PourStep pours[MAX_POURS];
    size_t pourCount;
};


extern const BrewProfile DEFAULT_PROFILE;


// Hardware control functions.

void pump(bool state);
void plate(bool state);
void setPourAngle(float angle);

void waitMs(uint32_t ms);


void pour(const PourStep& step);

void run();
void run(const BrewProfile& profile);


void onButtonPressed();
void onApiRequest();

}
