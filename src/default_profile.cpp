#include "v6o.hpp"

namespace v6o {

/**
 * The default brewing profile.
 */
const BrewProfile DEFAULT_PROFILE = {
    .initialDelayMs = 1000,

    .pours = {
        {
            .durationMs = 10000,
            .restAfterMs = 30000,

            .pump = {500, 500},
            .plate = {1000, 0},

            .angleStart = 10.0f,
            .angleEnd = 20.0f
        },

        {
            .durationMs = 15000,
            .restAfterMs = 20000,

            .pump = {700, 300},
            .plate = {1000, 0},

            .angleStart = 20.0f,
            .angleEnd = 15.0f
        },

        {
            .durationMs = 12000,
            .restAfterMs = 0,

            .pump = {500, 200},
            .plate = {1000, 0},

            .angleStart = 15.0f,
            .angleEnd = 10.0f
        }
    },

    .pourCount = 3
};

}
