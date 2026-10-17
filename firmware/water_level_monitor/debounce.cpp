#include "debounce.h"
#include "config.h"

Debouncer::Debouncer()
    : stableLevel_(false), pendingLevel_(false), pendingSinceMs_(0), hasPending_(false) {}

bool Debouncer::update(bool rawLevel, uint32_t nowMs) {
    if (rawLevel == stableLevel_) {
        hasPending_ = false; // raw agrees with the already-stable value; nothing to time
        return stableLevel_;
    }

    if (!hasPending_ || pendingLevel_ != rawLevel) {
        // A new candidate level just appeared (or flipped back and forth) — restart the timer.
        hasPending_ = true;
        pendingLevel_ = rawLevel;
        pendingSinceMs_ = nowMs;
        return stableLevel_;
    }

    // Same candidate level has held since pendingSinceMs_ — commit it once DEBOUNCE_MS passes.
    if (nowMs - pendingSinceMs_ >= DEBOUNCE_MS) {
        stableLevel_ = pendingLevel_;
        hasPending_ = false;
    }
    return stableLevel_;
}
