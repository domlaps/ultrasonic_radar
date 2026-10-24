# Problems and fixes

Each one: what I saw → what caused it → how I found it → how I fixed it.

## 1. Dead + rail on the breadboard — 2026-10-24

- **What I saw:** one of the positive power rails stopped working in the early wiring stage.
- **Cause:** I forced a resistor lead into the rail, which damaged the rail's internal metal
  contact strip.
- **How I found it:** I used the multimeter to check along the rail and find which part was
  damaged. It wasn't just one hole — the whole rail line was broken.
- **Fix:** replaced the breadboard. With a full rail gone, swapping it was more reliable
  than working around it.
- **Lesson:** never force a lead in. If it won't go, check the lead is straight and the hole
  is clear.

## 2. No way to mount the sensor on the servo — 2026-10-24

- **What I saw:** with the full circuit working, I had nothing to fix the HC-SR04 to the servo
  horn: no bracket, glue or foam tape.
- **Cause:** I planned the electronics in detail but never planned the mechanical side.
- **Fix:** taped the sensor to the servo horn. It works, but it isn't rigid.
- **Lesson:** plan the mount at the same time as the wiring. Next time: a proper HC-SR04 + SG90
  bracket, or a 3D-printed one.

## 3. Servo jitter during the sweep — 2026-10-24

- **What I saw:** the servo's movement was jittery instead of a smooth sweep.
- **Likely cause:** with 1° steps the servo starts and stops at every degree, so the motion
  is a long series of tiny jerks.
- **How I found it:** tried different rotation speeds and watched the movement.
- **Fix:** increased the speed of rotation, which gave a smooth, continuous movement.
  <!-- TODO: the exact change (step size / delay) and the values before and after -->

## 4. Occasional warning misfires and wrong distances — 2026-10-24 (still open)

- **What I saw:** rarely, the LEDs/buzzer jumped to the wrong zone for a moment, and now and
  then a distance reading was clearly wrong.
- **Likely cause:** the warning logic reacts to every single reading, so one bad echo is
  enough to trigger it. Bad echoes can come from angled or soft surfaces scattering the
  sound, an echo being missed, or the taped sensor wobbling as the servo moves.
- **How I found it:** watching the LEDs and the LCD during sweeps.
- **New lead from the datasheet:** the HC-SR04 datasheet recommends **at least 60 ms between
  measurements**, but the sketch pings about every 30 ms (the 30 ms `STEP_DELAY` plus the echo
  time). An echo from the previous ping can then arrive during the next one and be read as a
  false close object — and speeding the sweep up makes this more likely.
- **Status:** not fixed yet. Ideas to try: make sure at least 60 ms passes between pings, take
  the median of 3 readings at each angle, or only change the warning zone after 2 readings in a
  row agree.
  <!-- TODO: once tried, record what you changed and whether the misfires stopped -->

## 5. Schematic labels overlapping — 2026-10-24

- **What I saw:** when I exported the KiCad schematic to check it, some labels were
  unreadable: the servo's pin names were drawn on top of each other, the sensor's "GND" and
  "VCC" ran together as "GNDVCC", and the LCD's D0–D3 pin numbers touched the no-connect
  crosses.
- **Cause:** the custom symbols' boxes were too small for the pin names, and the unused LCD
  pins were too short for both a number and a cross.
- **Fix:** made the sensor and servo boxes taller, shortened the servo pin names
  (+5V / SIG / GND) and wrote the wire colours beside them, lengthened the LCD D0–D3 pins,
  and moved part names clear of the pin lines (Rev 3). No wiring changed — all 24
  connections were re-checked against the wiring plan.
