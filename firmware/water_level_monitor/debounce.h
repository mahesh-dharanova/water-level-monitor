#pragma once
#include <cstdint>

// Stabilizes a raw digital level stream: a new level must hold steady for
// DEBOUNCE_MS before it's accepted, rejecting capacitive-sensor chatter near
// the detection threshold. Pure C++, no Arduino dependency — unit-tested on
// the host.
class Debouncer {
public:
    Debouncer();

    // Feed the current raw level and timestamp, get back the stable state.
    bool update(bool rawLevel, uint32_t nowMs);

private:
    bool stableLevel_;
    bool pendingLevel_;
    uint32_t pendingSinceMs_;
    bool hasPending_;
};
