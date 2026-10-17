/*
  water-level-monitor — v1 sensor node + dashboard
  Board:  Arduino Uno R4 WiFi
  Sensor: XKC-Y25-PNP non-contact capacitive point-level sensor (see AGENTS.md)

  This file is an orchestrator only: setup()/loop() wire the modules together.
  All logic lives in sensor.*, debounce.*, health.*, history.*, reporting.*,
  network.* (see architecture.md). Full pipeline wiring lands in Chunk 6 (see
  plan.md); network/dashboard in Chunk 7.

  Chunk 2 (current): sensor + debounce are real; health/history/reporting are
  still stubs. The loop samples the raw pin every iteration (debounce needs
  frequent sampling to reject sub-second chatter) but only prints once per
  REPORT_INTERVAL_MS.
*/

#include "config.h"
#include "sensor.h"
#include "debounce.h"

static Debouncer debouncer;

void setup() {
  Serial.begin(115200);
  sensorInit();
}

void loop() {
  static uint32_t nextReportMs = 0;
  const uint32_t now = millis();

  const bool raw = sensorReadRaw();
  const bool stable = debouncer.update(raw, now);

  if (now < nextReportMs) return;
  nextReportMs = now + REPORT_INTERVAL_MS;

  Serial.print(F("{\"ms\":"));
  Serial.print(now);
  Serial.print(F(",\"raw\":"));
  Serial.print(raw ? 1 : 0);
  Serial.print(F(",\"stable\":"));
  Serial.print(stable ? 1 : 0);
  Serial.println(F("}"));
}
