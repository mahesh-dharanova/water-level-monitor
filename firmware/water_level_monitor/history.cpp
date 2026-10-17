#include "history.h"

PresenceHistory::PresenceHistory() {}

void PresenceHistory::record(uint32_t nowMs, bool stablePresence) {
    (void)nowMs;
    (void)stablePresence;
    // TODO(Chunk 4): push into the 5min raw ring buffer; every 60th call,
    // fold into the 24h hourly (1min summary) ring buffer.
}

uint16_t PresenceHistory::recentCount() const {
    // TODO(Chunk 4): real count once the ring buffer exists.
    return 0;
}

uint16_t PresenceHistory::hourlyCount() const {
    // TODO(Chunk 4): real count once the ring buffer exists.
    return 0;
}
