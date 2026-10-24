# Firmware

## Versions

| Version | Folder | Language | Notes |
|---|---|---|---|
| v1 | `radar/` | Arduino C++ | First working version. Written with help, then learned line by line. |
| v2 | <!-- TODO e.g. `radar_v2/` --> | <!-- TODO --> | <!-- TODO: my own rewrite — what changed and why --> |
| v3 | <!-- TODO e.g. `radar_c/` --> | <!-- TODO: plain C / AVR registers --> | <!-- TODO: optional --> |

How I learned to write these is in [journey/learning-c.md](../../journey/learning-c.md).

## Structure of v1

```
setup()  → configure pins, servo, serial and LCD once
loop()   → sweep 15° → 165° → 15°, forever
  scanAt(angle)
     move servo → wait 30 ms → readDistance()
     → updateWarnings() (LEDs + non-blocking buzzer)
     → updateLCD()
     → print "angle,distance." over serial
```

## Design choices

- **`const int` for pins and thresholds** so every setting is in one place at the top.
- **0 means "nothing detected".** `pulseIn` returns 0 on timeout, and the warning logic
  checks `d > 0` so "nothing there" is never treated as "very close".
- **Non-blocking buzzer.** `(millis() % period) < 75` switches the buzzer on for the first
  75 ms of each cycle. Using `delay()` instead would freeze the servo sweep.
- **Trailing spaces on the LCD.** The LCD doesn't clear old characters, so `"    "` after each
  value overwrites leftover digits (e.g. 165 → 99 would otherwise show 995).
- **Serial format `angle,distance.`** — the full stop marks the end of each reading, which is
  what common radar display sketches expect.

## Before and after (learning C)

<!-- TODO: once you've rewritten something yourself, put the two versions side by side here.
     Good candidates: readDistance(), updateWarnings(), or driving an LED with direct
     port registers (PORTC |= (1 << PC2)) instead of digitalWrite(). Explain in a few lines
     what you understand now that you didn't before. -->

## Known quirks

- `readDistance()` returns an `int`, so the distance is truncated to whole centimetres.
- The C++ parts are the `Servo`, `LiquidCrystal` and `Serial` objects; everything else
  (constants, loops, functions, maths) is valid C.
