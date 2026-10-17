#include "debounce.h"

Debouncer::Debouncer()
    : stableLevel_(false), pendingLevel_(false), pendingSinceMs_(0), hasPending_(false) {}

bool Debouncer::update(bool rawLevel, uint32_t nowMs) {
    (void)nowMs;
    // TODO(Chunk 2): real DEBOUNCE_MS-based stabilization, tuned against observed
    // chatter behavior on hardware. Stub passes the raw level through unchanged.
    stableLevel_ = rawLevel;
    return stableLevel_;
}
