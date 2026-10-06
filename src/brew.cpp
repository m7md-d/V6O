#include "v6o.hpp"

namespace v6o {

void pour(const PourStep& step)
{
    // Set the pour angle to the starting angle.
}

void run(const BrewProfile& profile)
{
    waitMs(profile.initialDelayMs);

    for (size_t i = 0; i < profile.pourCount; i++) {
        pour(profile.pours[i]);

        // If it's not the last pour,
        // wait for the rest period after the pour.
        if (i + 1 < profile.pourCount)
            waitMs(profile.pours[i].restAfterMs);
    }
}

void run()
{
    run(DEFAULT_PROFILE);
}

}
