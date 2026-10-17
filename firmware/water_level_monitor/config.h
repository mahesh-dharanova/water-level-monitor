#pragma once
#include <cstdint>

// Named constants shared by the pure logic modules and sensor.cpp/network.cpp.
// No magic numbers scattered in module bodies — change the behavior here.

// Pins
constexpr uint8_t SENSOR_OUT_PIN = 2; // yellow wire — see docs/HARDWARE_WIRING.md

// Timing
constexpr uint32_t REPORT_INTERVAL_MS = 1000;

// Debounce (Chunk 2) — placeholder until real chatter behavior is observed on hardware
constexpr uint32_t DEBOUNCE_MS = 50;

// Health (Chunk 3)
constexpr uint32_t HEALTH_WARMUP_MS      = 500;
constexpr uint32_t HEALTH_STUCK_MS       = 3600000UL; // 1h unchanged = suspicious for this use case
constexpr uint8_t  HEALTH_CHATTER_LIMIT  = 10;         // max transitions per HEALTH_CHATTER_WINDOW_MS
constexpr uint32_t HEALTH_CHATTER_WINDOW_MS = 1000;

// History (Chunk 4)
constexpr uint16_t HISTORY_RECENT_N = 300;  // last 5 min of 1s samples
constexpr uint16_t HISTORY_HOURLY_N = 1440; // last 24h of 1min summaries
