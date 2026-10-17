#pragma once
#include <cstdint>

// The only module allowed to touch Arduino.h / digitalRead() — see AGENTS.md.

// Configures the sensor's digital pin. Call once from setup().
void sensorInit();

// Reads the sensor's raw digital level (pre-debounce). Implemented in Chunk 2.
// true = OUT pin HIGH, false = OUT pin LOW. What that means for liquid
// presence depends on the MODE wiring — see docs/HARDWARE_WIRING.md.
bool sensorReadRaw();
