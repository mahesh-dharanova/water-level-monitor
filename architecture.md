# Architecture

Design and structure. Edit only on a real design decision.

## Overview

Single Arduino Uno R4 WiFi sensor node. `water_level_monitor.ino` is an orchestrator only;
all logic lives in single-concern modules under `firmware/water_level_monitor/`. The split
exists so everything that doesn't strictly need the Arduino runtime (`debounce`, `health`,
`history`, `reporting`) can be unit-tested on the host with plain `g++` — no emulator, no
new toolchain dependency. Only `sensor.*` (digital read) and `network.*` (WiFiS3) touch
hardware libraries and can only be verified on the physical board.

```
sensor.cpp --raw digital level--> debounce.cpp --stable presence--> history.cpp
 (digitalRead,                     (time-based                       (5min/24h
  Arduino.h only)                   stabilization, pure)               transition
                                           |                           ring buffers,
                                           v                           pure)
                                    health.cpp <-- independent signal       |
                               (chatter-rate / stuck /                     v
                                warm-up checks, pure,                reporting.cpp
                                never merged into the                (JSON builder,
                                presence reading itself)               pure)
                                                                           |
                                                                           v
                                                    Serial.println (USB) + network.cpp
                                                    (WiFiS3 HTTP server + dashboard,
                                                     Arduino-only, secrets.h credentials)
```

## Components

- **`sensor`** — reads the XKC-Y25-PNP's digital OUT pin. The only reading-side module with
  hardware dependence.
- **`debounce`** — time-based stabilization: a raw level must hold steady for
  `DEBOUNCE_MS` before it's accepted as the stable presence state, rejecting capacitive-
  sensor chatter near the detection threshold. Pure.
- **`health`** — plausibility checks independent of the presence reading itself:
  chatter-rate too high, stuck in one state too long, still warming up after power-on.
  Never merged with or inferred from the presence reading (house rule — same separation
  `tds-monitor` uses between `health` and `calibration`). Pure.
- **`history`** — ring buffers of presence/absence transitions: 5 minutes of 1-second
  samples, 24 hours of 1-minute summaries. Pure.
- **`reporting`** — builds the JSON report (serial line and HTTP response body) into a fixed
  buffer. Pure formatting, no I/O.
- **`network`** — WiFiS3 HTTP server: serves the dashboard and a JSON API (current state,
  history), mirrors the same output over USB serial. Reads WiFi credentials from
  `secrets.h` (gitignored). Hardware-dependent.

No `calibration.*` and no `storage.*`: a binary presence/absence reading has no quantity to
calibrate and nothing that needs to survive a reset in v1.

## Key decisions

| Decision | Why | Date |
|---|---|---|
| Sensor: XKC-Y25-PNP, powered at 5V (not its 5–12V upper range) | Keeps the PNP OUT pin's HIGH level at 5V, matching the Uno R4 WiFi's logic directly with no level-shifting — cross-checked against the instruction manual and three independent sellers this session, but the physical unit's label is still checked with a multimeter in Chunk 2 before fully trusting it | 2026-10-17 |
| MODE wire left floating by default (NO: OUT HIGH = liquid present) | Simpler default behavior; inverted (NC, MODE shorted to GND) documented as the alternative in `docs/HARDWARE_WIRING.md` but not wired unless Chunk 2 testing shows a reason to prefer it | 2026-10-17 |
| No `calibration.*` module | Binary presence/absence has no quantity to calibrate — v1 scope is point-level detection only, not continuous depth; continuous/multi-point sensing is v2 scope if it's ever needed | 2026-10-17 |
| No `storage.*`/EEPROM module in v1 | Nothing to persist without a calibration point or configurable threshold; added only if a real v2 need (e.g. a cross-reset alert setting) appears | 2026-10-17 |
| WiFi dashboard shipped in v1, not deferred | User's explicit choice this session, same v1-scope decision `tds-monitor` made over a serial-only phase | 2026-10-17 |
| Board: Arduino Uno R4 WiFi, FQBN `arduino:renesas_uno:unor4wifi` | User's hardware, consistent with `tds-monitor` and `turbidity-monitor` | 2026-10-17 |

## Open hardware questions (resolve during Chunk 2, don't assume)

- Whether the physical unit's MODE pin actually behaves as floating=NO / grounded=NC — wiring
  research this session found vendor listings for this sensor family disagree with each
  other, so this gets confirmed by direct observation, not trusted from a listing.
- The physical unit's exact rated supply voltage, read off its printed label — confirms the
  5V-powering decision above is safe for this specific unit before it's left running
  unattended.
- How long the debounce window (`DEBOUNCE_MS`) actually needs to be — set from observed
  chatter behavior at the threshold, not picked arbitrarily.
- Whether the sensor needs a settling/warm-up delay after power-on (affects
  `HEALTH_WARMUP_MS` in `config.h`).
