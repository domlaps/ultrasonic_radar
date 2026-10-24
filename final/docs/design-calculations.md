# Design calculations

All calculations done before building. Measured values go in the
[engineering log](../../journey/engineering-log.md) and [test-data](../test-data/README.md),
and are compared against these at the end.

## 1. Pin budget

| Part | Signal pins |
|---|---|
| HC-SR04 | 2 (Trig, Echo) |
| SG90 servo | 1 |
| LCD1602 (4-bit mode) | 6 (RS, E, D4–D7) |
| LEDs | 3 |
| Buzzer | 1 |
| **Total** | **13** |

The Uno has D0–D13 and A0–A5. D0 and D1 are the USB serial link, so they are left free.
That gives 12 digital + 6 analogue pins (the analogue pins work as digital I/O) = 18 available.
13 needed, 18 available, so it fits with 5 spare.

No part needs `analogWrite`, so the PWM pins (3, 5, 6, 9, 10, 11) are not a constraint.
The `Servo` library takes over Timer1, which disables PWM on pins 9 and 10. Pin 10 is only
used as a plain digital output (Trig), so this does not matter.

## 2. Distance from echo time

Sound travels to the object and back, so the one-way distance is half the round trip:

$$d = \frac{v \times t}{2}$$

With $v = 343$ m/s $= 0.0343$ cm/µs (air at about 20 °C):

$$d = \frac{0.0343 \times t}{2}$$

where $t$ is the echo time in µs and $d$ is in cm.

## 3. Echo timeout and maximum range

The code waits at most 12 000 µs for an echo:

$$d_{max} = \frac{0.0343 \times 12000}{2}$$

$$d_{max} = 205.8 \text{ cm}$$

So anything beyond about 2 m reads as "clear". This keeps each measurement short so the
sweep stays fast.

## 4. Sweep time

From 15° to 165° in 1° steps is 151 positions. Each position waits 30 ms for the servo, plus
up to 12 ms for the echo.

Best case (every echo returns immediately):

$$T = 151 \times 30 \text{ ms} = 4.53 \text{ s}$$

Worst case (every reading times out):

$$T = 151 \times (30 + 12) \text{ ms} = 6.34 \text{ s}$$

So one sweep in one direction should take between about 4.5 and 6.3 s.
<!-- TODO: measure it with a stopwatch or from serial timestamps and compare -->

## 5. Beam width

The HC-SR04's beam is roughly 15° wide (±7.5°). At distance $d$ the beam's width is

$$w = 2d \tan(7.5°)$$

At 1 m:

$$w = 2 \times 100 \times 0.1317$$

$$w \approx 26 \text{ cm}$$

So a thin object (like a pen) at 1 m is "seen" across about 15 servo steps and appears as an
arc. Stepping the servo by 1° is far finer than the beam, so a smaller step would not
improve the picture.

## 6. LED resistors

Uno pins output 5 V. A red LED drops about 2.0 V (yellow/green about 2.0–2.2 V). With 220 Ω:

$$I = \frac{V_{supply} - V_{LED}}{R}$$

$$I = \frac{5 - 2.0}{220}$$

$$I \approx 13.6 \text{ mA}$$

That is bright enough and under the ATmega328P's recommended 20 mA per pin (40 mA absolute max).
Total for all three LEDs if lit at once: about 41 mA, well under the chip's 200 mA total limit.
(In practice only one is lit at a time.)

## 7. LCD backlight

The LCD1602 backlight LED is fed through 220 Ω from 5 V. With a typical backlight forward
voltage of about 3.0–3.2 V:

$$I = \frac{5 - 3.1}{220}$$

$$I \approx 8.6 \text{ mA}$$

<!-- TODO: measure the real forward voltage across LCD pins 15–16 and recalculate -->

## 8. LCD contrast

The LCD-1602A datasheet recommends $V_{DD} - V_0 = 3.8$ V at 25 °C. With $V_{DD} = 5$ V:

$$V_0 = 5 - 3.8$$

$$V_0 = 1.2 \text{ V}$$

**Measured:** $V_0 = 1.278$ V, so $V_{DD} - V_0 \approx 3.72$ V, about 2 % from the
recommended value (assuming the rail is exactly 5.00 V).

## 9. Speed of sound vs temperature

$$v \approx 331.3 + 0.606 \, T \text{ m/s}$$

At 10 °C, $v \approx 337.4$ m/s; at 30 °C, $v \approx 349.5$ m/s. Using the 20 °C value at
those temperatures gives roughly a 2 % distance error (about 2 cm at 1 m). Small, but it's
a known limitation.

## 10. Servo power

An SG90 can draw several hundred mA when it starts moving or stalls. A laptop USB port
supplies about 500 mA in total for everything, so current spikes can make the 5 V rail dip,
resetting the Uno or flickering the LCD.

Mitigation: a 100 µF electrolytic capacitor across the servo's + and −, close to the servo,
to supply the short spikes locally.
<!-- TODO: record whether resets/flicker actually happened, and whether the cap fixed it -->
