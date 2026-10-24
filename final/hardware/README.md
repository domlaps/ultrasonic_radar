# Hardware

## Schematic

![Schematic](images/schematic.svg)

The same drawing is in three formats so anyone can open it, with or without KiCad:

| File | Best for |
|---|---|
| [`images/schematic.svg`](images/schematic.svg) | Viewing in the browser / on GitHub — stays sharp at any zoom |
| [`images/schematic.png`](images/schematic.png) | Quick preview, slides, LinkedIn (300 dpi) |
| [`images/schematic.pdf`](images/schematic.pdf) | Printing or zooming in any PDF viewer |
| [`kicad/radar.kicad_pro`](kicad/) | The editable KiCad project (opens in KiCad 8 or 9) |

The images are rendered straight from `kicad/radar.kicad_sch`, and every connection was
checked against the wiring tables below (24 nets, all matching). In KiCad, open
`kicad/radar.kicad_pro`; the project's own symbol library (`radar.kicad_sym`) loads
automatically through `sym-lib-table`, so no extra libraries are needed.

How to read it:

- The wire across the top is the breadboard **+ rail** (Uno 5V) and the wire across the
  bottom is the **− rail** (Uno GND). Every part connects straight onto them, the same way
  it does on the breadboard.
- Signal wires run straight from the Uno to each part: sensor and servo on the left, the
  LCD's six data wires on the right, buzzer and LEDs underneath.
- A **dot** means wires are joined. Where two wires cross without a dot (the sensor's GND
  passing the servo signal wire) they are **not** connected.
- Small red numbers are the physical pin numbers (LCD pins 1–16, sensor pins 1–4, servo
  wires 1–3). The servo's wire colours are written next to its pins.
- The blue ✕ marks on LCD D0–D3 mean "deliberately not connected" (the LCD runs in 4-bit mode).

## HC-SR04 ultrasonic sensor

| Sensor pin | Connects to |
|---|---|
| VCC | + rail (5 V) |
| Trig | D10 |
| Echo | D11 |
| GND | − rail |

## SG90 servo

| Wire | Connects to |
|---|---|
| Brown | − rail |
| Red | + rail (5 V) |
| Orange (signal) | D12 |

100 µF electrolytic capacitor across the servo's + and −, stripe (−) side to the − rail.

## LCD1602 (4-bit mode)

Pins numbered 1–16 from the left.

| LCD pin | Name | Connects to |
|---|---|---|
| 1 | VSS | − rail |
| 2 | VDD | + rail |
| 3 | V0 (contrast) | 10 kΩ pot wiper (outer legs to + and −) |
| 4 | RS | D7 |
| 5 | RW | − rail (write only) |
| 6 | E | D8 |
| 7–10 | D0–D3 | not connected |
| 11 | D4 | D5 |
| 12 | D5 | D4 |
| 13 | D6 | D3 |
| 14 | D7 | D2 |
| 15 | A (backlight +) | 220 Ω → + rail |
| 16 | K (backlight −) | − rail |

## LEDs

Long leg (anode) to the Uno pin, short leg → 220 Ω → − rail.

| LED | Pin | Meaning |
|---|---|---|
| Green | A0 | clear / over 50 cm |
| Yellow | A1 | 20–50 cm |
| Red | A2 | under 20 cm |

## Active buzzer

| Buzzer leg | Connects to |
|---|---|
| + (long) | D6 |
| − (short) | − rail |

## The finished build

![The finished build](images/final-build.jpg)

