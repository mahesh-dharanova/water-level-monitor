# water-level-monitor

Point-level liquid-presence sensor node: Arduino Uno R4 WiFi + an XKC-Y25-PNP non-contact
capacitive sensor.

## Overview

v1 reads the sensor's digital presence signal, debounces it against capacitive-sensor
chatter near the detection threshold, runs health checks, keeps a transition history, and
serves a dashboard over WiFi (and over USB as a fallback) — one JSON report per second plus
an HTTP API.

The sensor is **binary**: one unit reports presence/absence of liquid at one height, not a
continuous depth. There is no calibration step and no conversion curve — unlike this
project's sibling `tds-monitor`, which maps a continuous analog signal to a concentration.

## Goals

- v1: modular, tested Arduino Uno R4 WiFi sensor node — debounced, health-checked,
  presence/absence readings over serial, dashboard + JSON API over WiFi and USB.
- v2 (not started): continuous/multi-point level sensing if ever needed, additional sensor
  nodes, EEPROM-persisted settings if a cross-reset configuration need appears.

## Technology

- Arduino Uno R4 WiFi (`arduino:renesas_uno:unor4wifi`), Arduino core, C++.
- `arduino-cli` for compile/upload.
- Plain `g++` for host-side unit tests of the pure logic modules (no framework dependency).

## Setup

See `docs/HARDWARE_WIRING.md` for wiring and `scripts/compile.sh` / `scripts/upload.sh` for
the build workflow. Copy `firmware/water_level_monitor/secrets.h.example` to
`firmware/water_level_monitor/secrets.h` and fill in your WiFi credentials before Chunk 7
(network) lands — `secrets.h` is gitignored and never committed.

## Usage

Upload `firmware/water_level_monitor/water_level_monitor.ino`, open a serial monitor at
115200 baud, or browse to the dashboard address it prints once WiFi lands (Chunk 7).

## Development

Read `AGENTS.md` first — it's the authoritative, agent-agnostic rulebook for this repo.

## Testing

`tests/run_tests.sh` runs the host-side unit tests for the pure logic modules (`debounce`,
`health`, `history`, `reporting`). Hardware-in-loop verification is recorded in
`docs/TEST_LOG.md`.

## Configuration

No secrets in git. `firmware/water_level_monitor/secrets.h` (WiFi SSID/password, Chunk 7) is
gitignored; `secrets.h.example` is the committed template.

## Documentation

- `plan.md` — roadmap and chunks.
- `architecture.md` — module boundaries and design decisions.
- `current_state.md` — what's done, what's next (updated per verified chunk).
- `docs/` — hardware wiring, test log (`TEST_LOG.md` gets a dated entry the same session as
  every hardware trial — see `AGENTS.md`'s documentation model).
- Once real experimental data exists, write-ups get drafted in `mahesh-dharanova/publications`
  (three styles), promoted to `dharanova-code/publications` only after v1 is fully tested and
  demoed. This repo stays the engineering source of truth either way.
