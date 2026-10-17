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

- Chunk 2 (sensor + debounce): complete. Sensor wired (brown→5V, blue→GND, yellow→D2, black
  floating), `sensor.cpp` and `debounce.cpp` real and hardware-verified: clean dry baseline
  (~22s, `raw:0`/`stable:0`, no chatter), then a confirmed LOW→HIGH transition on real water
  contact (`stable` committed at `ms:56000`, held through the capture window), cross-checked
  against the sensor's own onboard LED by the user. **MODE open question resolved**:
  floating = NO confirmed on the physical unit, no inversion needed. See
  `docs/TEST_LOG.md` for the full dated entry.

## Next

- Chunk 3: health module — chatter-rate / stuck-too-long / warm-up checks, independent of
  the presence reading itself. No hardware session strictly required (pure logic, host
  tests), but a real session revisiting the HIGH→LOW recovery transition (not captured in
  Chunk 2's window) would close out the remaining open questions below.

## Open questions / blockers

- Sensor's exact printed supply-voltage rating not yet confirmed against the physical unit's
  label — the 5V-powering decision (see `architecture.md`) is working correctly so far, but
  the label check is still a to-do before leaving it running unattended long-term.
- Debounce window (`DEBOUNCE_MS=50` in `config.h`) wasn't stress-tested against
  threshold-edge chatter (e.g. water level sitting right at the detection boundary) — only
  clean single-edge transitions were observed so far.
- HIGH→LOW recovery transition (water removed) wasn't captured in Chunk 2's session window —
  not blocking, but worth confirming in a future hardware session.
- WiFi credentials not yet provided — `secrets.h` doesn't exist until Chunk 7; only
  `secrets.h.example` is committed.
