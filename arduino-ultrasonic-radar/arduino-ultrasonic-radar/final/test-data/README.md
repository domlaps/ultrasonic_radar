# Test data

Raw measurements and serial captures. Every number in the main README's Results table
should trace back to a file here.

## capture.py

Logs a sweep over serial and saves a CSV plus a polar plot.

```
pip install pyserial matplotlib
python3 capture.py <serial port> [number of sweeps]
```

Close the Arduino IDE's Serial Monitor first — only one program can use the port at a time.

## Measurements so far

| Date | What | Expected | Measured | Notes |
|---|---|---|---|---|
| 2026-10-24 | LCD contrast voltage V0 (pin 3 to GND) | ≈ 1.2 V (datasheet: VDD − V0 = 3.8 V) | 1.278 V | VDD − V0 ≈ 3.72 V assuming a 5.00 V rail, about 2 % from the datasheet value. Photo: [journey/photos/2026-10-24_04-lcd-v0-measurement.jpg](../../journey/photos/2026-10-24_04-lcd-v0-measurement.jpg) |

<!-- TODO: measure the real 5 V rail and the backlight voltage (LCD pins 15–16) and add them here -->

## Planned tests

<!-- TODO: do these, save the results here, and fill in the tables -->

### 1. Distance accuracy

Point the sensor straight at a flat object (a book or box) at fixed angle (servo held at 90°).
Measure the true distance with a tape measure, take 5 readings at each distance.
Save as `accuracy.csv`.

| True distance (cm) | Readings (cm) | Mean (cm) | Error (cm) |
|---|---|---|---|
| 10 | | | |
| 25 | | | |
| 50 | | | |
| 100 | | | |
| 150 | | | |
| 200 | | | |

### 2. Sweep time

Time several full sweeps from the CSV timestamps and compare with the calculated
4.5–6.3 s (see [docs/design-calculations.md](../docs/design-calculations.md)).

### 3. Scene scan

Place 2–3 objects at known angles and distances, capture a sweep, and include the polar
plot in the main README alongside a photo of the setup.

### 4. Warning zones

Move an object through 60 → 10 cm and confirm the LED/buzzer switch at 50 cm and 20 cm.
