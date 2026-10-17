# Test Log

Dated hardware-in-loop verification entries. Never edit a past entry — append a new one.

## 2026-10-17 — Chunk 1 scaffold

- `scripts/compile.sh` succeeds against `arduino:renesas_uno:unor4wifi` with the empty
  orchestrator (`Serial.begin()` only) and all stub modules compiled alongside it.
- `tests/run_tests.sh` passes (asserts stub placeholder behavior only — not real logic).
- No hardware connected yet; no sensor wiring performed.

## 2026-10-17 — Chunk 2: first real hardware session

- Sensor wired per `docs/HARDWARE_WIRING.md`: brown→5V, blue→GND, yellow→D2, black left
  floating (default NO).
- `sensor.cpp` (`digitalRead(SENSOR_OUT_PIN)`) and `debounce.cpp` (time-based stabilization,
  `DEBOUNCE_MS=50`) implemented for real; `tests/run_tests.sh` passes with real debounce
  assertions (sustained-level commit, chatter rejection, both directions).
- Uploaded and monitored over USB serial (`/dev/ttyACM0`, 115200 baud). Captured JSON report
  stream (`{"ms":...,"raw":...,"stable":...}` once per second):
  - Dry baseline held clean and steady at `raw:0`/`stable:0` for ~22s (`ms:33000`–`55000`),
    no chatter.
  - User brought water to the sensor; `stable` committed to `1` at `ms:56000` and held
    steady at `raw:1`/`stable:1` through the end of the 40s capture window (`ms:69000`).
  - User visually confirmed the sensor's own onboard LED behavior matched the serial output
    throughout: LED off while dry, LED on once water was presented — independent
    confirmation that the digital read is tracking the sensor's actual detection state, not
    an artifact of the firmware.
  - Capture window ended before the user removed the water again, so the HIGH→LOW recovery
    transition wasn't captured this session — not blocking, since the LOW→HIGH transition
    and the sustained dry/wet baselines are the parts that confirm MODE behavior.
- **Resolves the MODE open question**: floating MODE wire = NO (OUT LOW at rest, HIGH on
  liquid detection), confirmed on the physical unit — matches the documented default, no
  inversion needed.
- Still open: the unit's printed supply-voltage rating wasn't read off the physical label
  this session (powering at 5V is working correctly, but the label check is still a
  to-do — see `current_state.md`). Debounce window (`DEBOUNCE_MS=50`) wasn't specifically
  stress-tested against threshold-edge chatter (e.g. water level right at the detection
  boundary) — this session's transitions were both clean single edges, so 50ms is adequate
  for what was observed, but not proven as a worst-case value.
