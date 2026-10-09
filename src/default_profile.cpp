#include "v6o.hpp"

namespace v6o {

/**
 * The default brewing profile.
 */
const BrewProfile DEFAULT_PROFILE = {
    .pours = {
        {
            .waitBeforeMs = 0,
            .durationMs = 10000,
            .pumpPulse = { .onMs = 3000, .offMs = 6000 }
        },

        {
            .waitBeforeMs = 30000,
            .durationMs = 15000,
            .pumpPulse = DEFAULT_PUMP_PULSE
        },

        {
            .waitBeforeMs = 20000,
            .durationMs = 12000,
            .pumpPulse = DEFAULT_PUMP_PULSE
        }
    },

    .pourCount = 3
};

}
