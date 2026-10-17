# Current State

Updated once per chunk, right after it is built and verified.
This is a pointer into git history for the next agent, not a duplicate of it.

## Done

- Chunk 1 (scaffold): repo structure, `AGENTS.md`/`README.md`/`plan.md`/`architecture.md`,
  stub modules (`sensor`, `debounce`, `health`, `history`, `reporting`, `network`) with
  signatures only, `tests/run_tests.sh` harness, `scripts/compile.sh`/`upload.sh`, doc
  stubs, `secrets.h.example`. Verified: `scripts/compile.sh` compiles the empty orchestrator
  against `arduino:renesas_uno:unor4wifi`; `tests/run_tests.sh` runs (stub assertions only —
  all modules are stubs pending Chunk 2's hardware session).

## Next

- Chunk 2: wire the XKC-Y25-PNP to the board (brown→5V, blue→GND, yellow→D2, black
  floating), implement `sensor.cpp`/`debounce.cpp`, confirm the MODE pin's real NO/NC
  behavior and the unit's printed supply-voltage rating on hardware, log a
  dry/liquid-present session in `docs/TEST_LOG.md`. Needs the physical board + sensor in
  hand.

## Open questions / blockers

- MODE pin's actual NO/NC behavior on this physical unit is unverified — vendor listings for
  this sensor family disagree with each other (see `architecture.md`'s open questions);
  Chunk 2 must observe it directly.
- Sensor's exact printed supply-voltage rating not yet confirmed against the physical unit's
  label — the 5V-powering decision (see `architecture.md`) assumes it's safe, pending that
  check.
- Debounce window (`DEBOUNCE_MS` in `config.h`) is a placeholder until Chunk 2 observes real
  chatter behavior at the threshold.
- WiFi credentials not yet provided — `secrets.h` doesn't exist until Chunk 7; only
  `secrets.h.example` is committed.
