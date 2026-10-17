#include "health.h"

HealthMonitor::HealthMonitor() {}

HealthStatus HealthMonitor::check(bool stablePresence, uint32_t nowMs) {
    (void)stablePresence;
    (void)nowMs;
    // TODO(Chunk 3): chatter-rate / stuck-too-long / warm-up checks.
    return HealthStatus::Ok;
}
