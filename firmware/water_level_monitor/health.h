#pragma once
#include <cstdint>

// Plausibility signal, independent of the presence reading itself — see
// AGENTS.md / architecture.md ("never merge health and the reading").
enum class HealthStatus {
    Ok,
    Chattering,
    Stuck,
    WarmingUp,
};

class HealthMonitor {
public:
    HealthMonitor();

    // Call once per report cycle with the current stable (debounced) presence state.
    HealthStatus check(bool stablePresence, uint32_t nowMs);
};
