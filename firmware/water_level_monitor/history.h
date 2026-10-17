#pragma once
#include <cstdint>
#include "config.h"

// Ring buffers of presence/absence transitions: HISTORY_RECENT_N seconds of
// raw samples, HISTORY_HOURLY_N minutes of 1-minute summaries. Pure C++, no
// Arduino dependency — RAM-only, resets on power cut (no persistence; v1 has
// no storage.* module — see architecture.md).
class PresenceHistory {
public:
    PresenceHistory();

    // Call once per report cycle (REPORT_INTERVAL_MS) with the current stable state.
    void record(uint32_t nowMs, bool stablePresence);

    uint16_t recentCount() const;
    uint16_t hourlyCount() const;
};
