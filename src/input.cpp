#include "v6o.hpp"

namespace v6o {

/**
 * Physical action.
 */
void onButtonPressed()
{
    run();
}

/**
 * Logical action.
 */
void onApiRequest(const BrewProfile& profile)
{
    run(profile);
}

}
