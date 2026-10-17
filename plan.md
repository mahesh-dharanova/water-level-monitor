# Plan

Roadmap broken into chunks. Edit only on a real scope decision.

## Goals

- v1: a modular, tested Arduino Uno R4 WiFi sensor node that reports debounced,
  health-checked liquid presence/absence from an XKC-Y25-PNP point-level sensor, over
  serial JSON and a WiFi dashboard/HTTP API.
- v2 (not started): continuous/multi-point level sensing if ever needed, additional sensor
  nodes, `storage.*`/EEPROM if a cross-reset setting appears.

## v1 (current)

1. [x] Scaffold — repo structure, docs, stub modules, git init.
   Verified: `scripts/compile.sh` succeeds on the empty orchestrator.
2. [x] Sensor + debounce modules — wired the XKC-Y25-PNP (brown→5V, blue→GND, yellow→D2,
   black floating), `sensor.cpp` (digitalRead) and `debounce.cpp` (time-based
   stabilization) real and hardware-verified: clean dry baseline, confirmed LOW→HIGH on
   real water contact, MODE pin's NO behavior confirmed (floating = NO, no inversion
   needed).
   Verified: `tests/run_tests.sh` covers debounce edge cases; upload + monitor, a
   dry/liquid-present session logged in `docs/TEST_LOG.md`.
3. [ ] Health module — chatter-rate / stuck-too-long / warm-up checks, independent of the
   presence reading itself.
   Verified: one unit test per fault condition.
4. [ ] History module — 5-minute (1s) and 24-hour (1min) transition ring buffers.
   Verified: unit tests for wraparound and transition counting.
5. [ ] Reporting module — JSON line builder (serial + HTTP body).
   Verified: unit tests on formatting and edge values.
6. [ ] Orchestrator wiring, serial-only — `.ino` calls sensor → debounce → health → history
   → reporting → `Serial.println`, once per second. No network yet — validates the whole
   pipeline on real hardware before adding networking complexity on top of still-unverified
   assumptions.
   Verified: compile + upload + monitor, dry and liquid-present session logged.
7. [ ] Network module + dashboard — WiFiS3 HTTP server, `secrets.h`-based WiFi credentials,
   a new dashboard (built for this project) + JSON API, mirrored over USB serial.
   Verified: hardware session — dashboard reachable over WiFi and over USB, logged in
   `docs/TEST_LOG.md`.
8. [ ] Integration pass — full host test suite + fresh end-to-end hardware session together,
   `current_state.md` matches reality, tag v1.

## v2 (not started)

- Continuous or multi-point level sensing, if a real need for it appears (e.g. multiple
  XKC-Y25-PNP units at different heights, or a different sensor type for true depth).
- `storage.*`/EEPROM module, if a cross-reset configuration need appears (e.g. a
  user-adjustable alert threshold).
- Re-verify against the host tests and a fresh hardware session.
