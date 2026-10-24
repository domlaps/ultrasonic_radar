# Learning C alongside the build

**Starting point (2026-10-24):** beginner programmer. I can read and explain the radar sketch
line by line, but it was written with help. Goal: write the radar code myself, then in C.

## Progress

| Date | What I learned | How it shows in the radar code |
|---|---|---|
| 2026-10-24 | Arduino = C++ with `setup()`/`loop()` on top; difference between C and C++ features | Spotted which parts of `radar.ino` are C++ objects vs C-style code |
| 2026-10-24 | Objects and constructors, global vs local scope, `const`, `bool`, `%` (modulo), `unsigned long` for times | Understood every section of `radar.ino`; ready to write Lesson 1 myself |

## Lesson plan (each lesson = one build stage, my own code)

- [ ] Lesson 1: HC-SR04 only → distance in Serial Monitor; ruler comparison; what happens with no object in range
- [ ] Lesson 2: servo sweep
- [ ] Lesson 3: combine — measure at every angle
- [ ] Lesson 4: LEDs + buzzer (`if`/`else`, non-blocking beeps)
- [ ] Lesson 5: LCD — wiring and HelloWorld test done; write my own angle/distance display
- [ ] Lesson 6: tidy into functions; compare with the v1 sketch in [final/firmware](../final/firmware/)

## C milestones

- [ ] Core C: variables/types, functions, `#define` vs `const`, arrays
- [ ] Pointers, structs, bit manipulation, memory
- [ ] Rewrite one radar function myself without help (e.g. `readDistance()`)
- [ ] Rewrite the radar logic in plain C style (direct register access for an LED, no Arduino objects where possible)
- [ ] Compare my version with the original — note what I'd now do differently
