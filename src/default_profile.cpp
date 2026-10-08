#include "v6o.hpp"

namespace v6o {

/**
 * The default brewing profile.
 */
const BrewProfile DEFAULT_PROFILE = {
    .pours = {
        {
            .waitBeforeMs = 0,
            .durationMs = 10000
        },

        {
            .waitBeforeMs = 30000,
            .durationMs = 15000
        },

        {
            .waitBeforeMs = 20000,
            .durationMs = 12000
        }
    },

    .pourCount = 3
};

}
