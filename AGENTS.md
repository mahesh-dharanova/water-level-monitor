# AGENTS.md

Rules for any agent working in this repo (Claude Code, Codex, OpenCode with a local model,
others). Work here may switch agents mid-task: everything needed to continue lives in this
repo, never in an agent's private memory. `CLAUDE.md` in this repo just points here.

water-level-monitor is a single Arduino Uno R4 WiFi sensor node (XKC-Y25-PNP non-contact
capacitive point-level sensor). v1 scope: debounced, health-checked presence/absence
readings, served over serial JSON **and** a WiFi dashboard/HTTP API. No calibration and no
EEPROM persistence in v1 — there's no quantity to calibrate and nothing that needs to
survive a reset yet. See `plan.md` for the full roadmap and `architecture.md` for why the
code is split the way it is.

## If you are a small model (1.5B to 3B, 16k context)

- One chunk per session. Read only `current_state.md` and the one module you're touching.
- Copy an existing module's pattern (e.g. a new pure-logic module: copy `health.h`/`health.cpp`
  and its test) instead of inventing a new shape.
- Unsure? Stop and say exactly what you do not understand. Never guess a fix.

## Where the truth lives

- Progress: `git log --oneline -15`, then `current_state.md`.
- Roadmap: `plan.md` (stable; only changes on a real scope decision).
- Design/module boundaries: `architecture.md` (stable; only changes on a real design decision).
- Hardware-in-loop results: `docs/TEST_LOG.md` (dated entries, never overwritten, written the
  same session the data was captured — not reconstructed later from memory).
- Do not create HANDOFF or status files. Continuity is `current_state.md` + the commits.

## Documentation model (three tiers — see `~/Dev/CLAUDE.md`'s workspace conventions)

1. **This repo** — the engineering source of truth. `docs/TEST_LOG.md` gets a dated entry
   the same session as every hardware trial (readings, observations, pass/fail), whether or
   not it's the outcome hoped for. `current_state.md` updates in the same commit as every
   verified chunk. Nothing here waits for the project to finish.
2. **`mahesh-dharanova/publications`** (middle tier, informal) — once a chunk produces real
   experimental data worth a write-up (starting around Chunk 2's hardware session), draft it
   there under a new topic folder, in the three house styles (`templates/dharanova-strict`,
   `templates/academic-imrad`, `templates/dharanova-narrative`). See that repo's own
   `README.md`/`templates/README.md`.
3. **`dharanova-code/publications`** (official) — only the `dharanova-strict` draft moves
   here, only after v1 is fully tested and demoed, per that repo's `PUBLISHING_GUIDELINES.md`.
   Never fabricate a number; every figure traces to a file in `data/` or is labelled
   calculated/simulated/datasheet.

## Module boundary (the one rule that matters most here)

`firmware/water_level_monitor/water_level_monitor.ino` is an orchestrator ONLY:
`setup()`/`loop()` calling into modules, no logic of its own. Every module is one concern:

- `sensor.*`, `network.*` — the only modules allowed to touch `Arduino.h` / hardware
  libraries (`digitalRead()`, `WiFiS3`).
- `debounce.*`, `health.*`, `history.*`, `reporting.*` — plain C++, no `Arduino.h`, so they
  compile and run as real unit tests on the host with `g++` (`tests/run_tests.sh`).
- `health` (is the sensor behaving plausibly — chatter, stuck, still warming up?) is
  independent of the presence reading itself. Never infer sensor health from what the
  reading currently says, or vice versa — same separation principle `tds-monitor` uses
  between `health` and `calibration`.

## Commands (repo root)

- Host unit tests (fast, no hardware): `tests/run_tests.sh`
- Compile for the board (no upload): `scripts/compile.sh`
- Upload (board connected): `scripts/upload.sh /dev/ttyACMx`
- Monitor: `arduino-cli monitor -p /dev/ttyACMx --config baudrate=115200`

## Chunk protocol

1. Say the goal and how you will verify it, in two lines.
2. Bug: write the failing test first. Feature: its test comes in the same chunk.
3. Smallest change that works, in the style of the surrounding code. No drive-by edits.
4. Verify: `tests/run_tests.sh` for pure modules; `scripts/compile.sh` always; hardware
   session + `docs/TEST_LOG.md` entry for anything touching `sensor.*`, `network.*`, or the
   `.ino`.
5. Update `current_state.md` in the same commit: what's done, what's next.
6. Commit after every verified chunk (message = what changed, how verified, what's open).
   Pushing needs the user's explicit go-ahead the first time a remote exists.

## Never

- Secrets in files. WiFi credentials live in `firmware/water_level_monitor/secrets.h`, which
  is gitignored — `secrets.h.example` is the committed template. Never hardcode SSID/password
  in the `.ino` or any committed file.
- Hardcoded machine values (paths, usernames). Pin numbers and board constants belong in
  `config.h` as named constants, not scattered literals.
- Claiming the MODE wiring's NO/NC behavior, the sensor's exact supply voltage, or the
  debounce window without having observed it on real hardware first — see the open
  questions in `architecture.md`.
- Weakening or deleting a test to get green. If a host test fails, the module is wrong.
- `git push`, `reset`, `rebase`, force anything, without the user's explicit approval.

## Too big or risky for a small model: stop and say so

Anything that would turn this into continuous/multi-point level sensing (v2, not v1),
anything touching WiFi credential handling, and anything needing hardware you don't have
physical access to verify.

## Hard-won facts

- Sensor is XKC-Y25-PNP, confirmed by the user — not the XKC-Y25-V/Y26 variants an earlier
  web search for "VCC/OUT/GND/MODE" wiring also turned up. Listings for this sensor family
  disagree with each other on MODE pin behavior across vendors, so it's treated as unverified
  until Chunk 2 observes it directly.
- Wiring (per the instruction manual and three independent sellers, cross-checked this
  session — see `docs/HARDWARE_WIRING.md`): brown=VCC, blue=GND, yellow=OUT (PNP, drives
  HIGH=VCC level when active), black=MODE (floating=NO, shorted to blue/GND=NC/inverted).
  VCC is powered at 5V (not the 5–12V upper range) specifically so OUT's HIGH level stays at
  5V, matching the Uno R4 WiFi's logic directly with no level-shifting — confirm against the
  physical unit's printed label with a multimeter before fully trusting this in Chunk 2.
- No calibration and no EEPROM/`storage.*` module in v1 — a binary presence/absence reading
  has no quantity to calibrate and nothing that needs to survive a reset. Don't add either
  back in without a real v2 need.
