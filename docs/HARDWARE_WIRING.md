# Hardware Wiring

## Board

Arduino Uno R4 WiFi (`arduino:renesas_uno:unor4wifi`). Digital I/O at 5V.

## Sensor

XKC-Y25-PNP non-contact capacitive point-level sensor. Confirmed by the user — not the
XKC-Y25-V/Y26 variants an initial web search for "VCC/OUT/GND/MODE" wiring also turned up;
those have different (and vendor-inconsistent) documented behavior and don't apply here.

Wiring below is cross-checked across the instruction manual and three independent sellers
(icstation, Gleanntronics, Sharvi Electronics) this session — they agree on wire colors and
PNP output behavior. Still to confirm against the physical unit in Chunk 2 (see below).

```
          brown (VCC) ----> 5V
  [XKC-Y25-PNP] ---blue (GND) ----> GND        [Arduino Uno R4 WiFi]
          yellow (OUT) ---> D2 (digital input)
          black (MODE) ---> floating (default) or shorted to blue/GND (inverted)
```

| Wire | Function | Connects to | Notes |
|---|---|---|---|
| Brown | VCC | Uno R4 WiFi **5V** pin | Sensor is commonly rated 5–12V; powering it at 5V (not higher) keeps the PNP OUT pin's HIGH level at 5V too, matching the board's logic with no level-shifting. **Confirm the unit's printed voltage rating with a multimeter before trusting this.** |
| Blue | GND | Uno R4 WiFi **GND** | |
| Yellow | OUT (PNP signal) | Any digital pin, e.g. **D2** | PNP actively drives HIGH=VCC-level when active, LOW=0V when not — `pinMode(D2, INPUT)`, not `INPUT_PULLUP`. If readings float during Chunk 2, that's the signal to add a pull-down resistor, not something to assume now. |
| Black | MODE (NO/NC select) | Leave **floating** for default NO (OUT HIGH when liquid detected), or short to **blue/GND** for inverted NC (OUT LOW when liquid detected) | Logic-select line, not a secondary ground — the manual explicitly warns against using it as GND even though shorting it to GND is how inversion works. |

The sensor gives **one threshold, present/absent** — not a continuous depth reading. One
unit = one point-level switch.

## Logic-level check

PNP output swings between 0V and VCC. Powering VCC at 5V keeps OUT's HIGH level at 5V,
within the Uno R4 WiFi's native 5V digital I/O — no level-shifting needed as currently
understood. **Do not power this sensor from 12V** unless a resistor divider or level-shifter
is added on OUT first (per `Embedded/CLAUDE.md`'s standing rule to proactively flag
voltage mismatches).

## To confirm in Chunk 2

- [ ] Physical wire colors and connector on the actual unit match this table.
- [ ] The unit's printed supply-voltage rating (confirms 5V powering is within spec).
- [ ] MODE pin's real NO/NC behavior with black floating vs. shorted to GND — observed
      directly, not assumed from a listing (vendor listings for this sensor family disagree
      with each other on this point).
- [ ] Observed OUT behavior dry (no liquid) vs. liquid-present, recorded in `TEST_LOG.md`.
- [ ] Whether the sensor needs a settling/warm-up delay after power-on.
- [ ] How much chatter/bounce OUT shows near the detection threshold — sets `DEBOUNCE_MS` in
      `config.h` from observation, not a guess.
