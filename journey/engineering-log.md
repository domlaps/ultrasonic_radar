# Engineering log

Day-by-day record of the build, including mistakes. Newest entries at the bottom.
Photos are in [photos/](photos/); faults are written up in [problems-and-fixes.md](problems-and-fixes.md).

## Board choice and design

- Chose the **Arduino Uno R3**. The HC-SR04 and SG90 both run on 5 V directly, and radar
  display sketches expect the standard Arduino serial format.
- Finalised the parts: HC-SR04, SG90, LCD1602 with a 10 kΩ contrast pot, red/yellow/green
  LEDs with 220 Ω resistors, active buzzer, 100 µF capacitor for the servo. All from the
  Elegoo Uno starter kit and a Bojack 480-piece kit.
- **Pin planning:** 13 signal pins needed, 18 available after leaving D0/D1 free for USB
  serial. The `Servo` library takes Timer1 (PWM on 9/10), which is fine because pin 10 is a
  plain digital Trig output. Details in
  [design-calculations.md](../final/docs/design-calculations.md).
- **Resistor sizing:** 220 Ω gives about 13.6 mA per LED — under the LED and pin limits.
- **Code v1:** servo sweep with `pulseIn` distance measurement, three warning zones,
  non-blocking buzzer with `millis()`, LCD readout and a serial `angle,distance.` stream.
- **Risk identified:** servo current spikes on USB power could reset the Uno or flicker the
  LCD. Plan: a 100 µF capacitor across the servo supply.
- Went through the code line by line: `LiquidCrystal` and `Servo` objects, why the LCD
  needs trailing spaces, how the `millis()` beep timing works.
- **Language check:** worked out that the sketch is C++ (the `Servo`, `LiquidCrystal` and
  `Serial` objects) with the rest being C-compatible. Decided to learn C properly, since it
  is the standard language for microcontroller firmware — see [learning-c.md](learning-c.md).
- **LCD datasheet check** (LCD-1602A, SunFounder):
  - R/W tied to GND because the radar only writes; the library waits fixed times instead of
    reading the busy flag.
  - VDD 4.5–5.5 V operating, 7 V absolute max, so the Uno's 5 V is safe.
  - Contrast: recommended VDD − V0 = 3.8 V at 25 °C, so V0 ≈ 1.2 V.
  - Backlight pin 15 is listed at +5.0 V with no current given, which suggests a built-in
    resistor. Kept the 220 Ω anyway as extra protection.
- **Schematic drawn in KiCad** ([final/hardware/kicad](../final/hardware/kicad/)). First
  version joined parts with net labels; redrawn (v2) with every connection as a real wire:
  the +5V and GND nets are drawn as rails across the top and bottom, like the breadboard, and
  every part is wired onto them. Signal wires run straight from the Uno to each part (pin
  positions on the symbols chosen so they line up). One unavoidable crossing (the sensor's GND
  wire passes the servo signal wire, no dot = not joined). Checked geometrically: all
  connections match the wiring plan, junction dots only where wires actually join, no wire
  passes through a part, LCD D0–D3 marked no-connect.

## Power rails, LCD power and contrast

![First wiring attempt](photos/2026-10-24_01-first-wiring-attempt.jpg)

- First layout of the Uno, breadboard and LCD.
- **Damaged breadboard rail:** forcing a resistor lead into one of the + rails damaged it.
  I traced it with the multimeter, found the whole rail line was broken, and swapped to a
  new breadboard. Full write-up in [problems-and-fixes.md](problems-and-fixes.md).

![Power rails and LCD backlight](photos/2026-10-24_02-power-rails-and-lcd-backlight.jpg)

- **Power:** 9 V battery into the Uno's barrel jack; breadboard rails fed from the Uno's 5V
  and GND pins, with both sides' rails bridged at the top. Checked that the LCD only sees the
  5 V rail (its absolute max is 7 V), not the raw 9 V.
- **Backlight on** through the 220 Ω resistor.

![Contrast pot fitted](photos/2026-10-24_03-contrast-pot-fitted.jpg)

- **Contrast pot wired** (outer legs to 5 V and GND, wiper to V0). With no code running, the
  top line shows solid blocks — the expected state of an HD44780 that is powered but not yet
  initialised, and proof the contrast adjustment works.

![Measuring V0](photos/2026-10-24_04-lcd-v0-measurement.jpg)

- **Measured V0 = 1.278 V** (Crenova MS8233D, DC V, LCD pin 3 to GND). Datasheet target
  ≈ 1.2 V. VDD − V0 ≈ 3.72 V assuming a 5.00 V rail, within about 2 % of the recommended
  3.8 V. Still to do: measure the real 5 V rail so the comparison uses the actual VDD.

## LCD wiring test

![LCD HelloWorld test](photos/2026-10-24_05-lcd-helloworld-test.jpg)

- Wired RS, E and D4–D7 to the Uno and uploaded the IDE's LiquidCrystal → HelloWorld example.
- "hello, world!" on line 1 and a seconds counter on line 2. This confirms the six signal
  wires, 4-bit mode, contrast and power before adding anything else. The block row
  disappeared once `lcd.begin()` ran.

## LEDs and buzzer wired

![LEDs and buzzer wired](photos/2026-10-24_06-leds-and-buzzer-wired.jpg)

- Added the red, yellow and green LEDs, each with its 220 Ω resistor, and the active buzzer,
  and wired them to the Uno (LEDs to A0–A2, buzzer to D6 per the wiring plan).
- Photo taken with the board unplugged, so nothing is lit yet. Still to do: power up and
  check each LED and the buzzer switches on from its pin.

## Full circuit wired

![Full circuit wired](photos/2026-10-24_07-full-circuit-wired.jpg)

- Added the last parts: the HC-SR04 (Trig D10, Echo D11) and the SG90 servo (signal D12) on
  jumper leads so they can move, and the 100 µF electrolytic capacitor across the servo's
  supply. Every part from the wiring plan is now connected.
- Photo taken unpowered. The sensor isn't mounted on the servo horn yet.
- Still to do before the first full run: check the capacitor's stripe (−) is on the − rail,
  then power up and test each part on its own (LEDs, buzzer, sensor readings, servo sweep).

## First full run: everything works

![The radar reacting to my hand](../final/media/sweep.gif)

- Uploaded the reference sketch (`radar.ino`) from the Arduino IDE and powered up the
  complete circuit. **It worked fully on the first full run** — sweep, distance, LCD, LEDs
  and buzzer all working together.
- **Mounting:** taped the sensor to the servo horn. I had no bracket or glue because I
  hadn't planned the mount in advance — a lesson for next time
  ([problems-and-fixes.md](problems-and-fixes.md) #2).
- **Glitches seen:**
  - Servo jitter during the sweep — fixed by increasing the speed of rotation, which gave a
    smooth continuous movement (#3).
  - Rare warning misfires and rare wrong distance readings — not fixed yet (#4).
- **Tests and tuning:** varied the motor speed and the warning distances, and improved the
  code with Claude's help. Result: all the radar's functions worked more efficiently and
  faster. <!-- TODO: the values tried, the values chosen, and the improved sketch as v2 -->
- **Demo recorded:** a ~10 s clip of my hand moving in front of the sensor with the LEDs and
  LCD reacting. The original was 52 MB, too big to show in a GitHub README, so the repo copy
  (`final/media/sweep.gif`) is scaled to 480 px at 12 fps (7.8 MB). Using one colour palette
  for the whole GIF tinted every frame green whenever the green LED was lit, so each frame
  gets its own palette.

## Finished-build photo and schematic Rev 3

![The finished build](../final/hardware/images/final-build.jpg)

- Took a top-down photo of the finished build
  ([final/hardware/images/final-build.jpg](../final/hardware/images/final-build.jpg)).
- Exported the schematic from KiCad 9.0.1 to check it and found label clashes: the servo's pin
  names drawn on top of each other, "GND" and "VCC" on the sensor running together, the LCD's
  D0–D3 pin numbers touching the no-connect crosses, and part names crossing pin lines.
- Fixed them in the KiCad files (Rev 3) without changing any wiring
  ([problems-and-fixes.md](problems-and-fixes.md) #5). All 24 connections re-checked against
  the wiring plan.
- The schematic is now in the repo as SVG, PNG and PDF
  ([final/hardware/images](../final/hardware/images/)), so it can be opened without KiCad.

<!-- Next entries: measured results (distance accuracy, sweep time, zone switch-over), the
     laptop radar display, and my own rewrite of the code. For each: what I did, what I
     measured, what went wrong, how I found the cause, how I fixed it. -->
