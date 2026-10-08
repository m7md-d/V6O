#include "v6o.hpp"

namespace v6o {

void pour(const PourStep& step)
{
    // Set the pour angle to the starting angle.
}

void run(const BrewProfile& profile)
{
    for (size_t i = 0; i < profile.pourCount; i++) {
        waitMs(profile.pours[i].waitBeforeMs);
        pour(profile.pours[i]);
    }
}

void run()
{
    run(DEFAULT_PROFILE);
}

}
