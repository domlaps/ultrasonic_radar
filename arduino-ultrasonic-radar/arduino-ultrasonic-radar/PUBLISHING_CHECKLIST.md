# Before publishing — delete this file when done

## Fill in
- [ ] Every `<!-- TODO -->` in README.md, final/ and journey/
- [ ] Results table — only numbers you actually measured, with the raw data in final/test-data/
- [ ] "Problems I ran into" — 3–6 real ones (from journey/problems-and-fixes.md), each with what you saw → cause → how you found it
- [ ] "About this project" — accurately says what was done with help and what you did yourself
- [ ] Engineering log refreshed with the final entries (ask Claude to regenerate it from the project log)

## Media (the most important part for recruiters)
- [x] `final/media/sweep.gif` — radar reacting to a hand (compressed copy: 480 px, 12 fps, 7.8 MB; added to the README)
- [x] `final/hardware/images/final-build.jpg` — top-down photo of the finished build
- [ ] One photo per build stage in `journey/photos/`, named `YYYY-MM-DD_NN-what-it-shows.jpg`, and added to journey/README.md
  - [x] LCD stage (power, contrast, V0 measurement, HelloWorld)
  - [x] LEDs/buzzer wired, full circuit wired
  - [x] Final build
- [x] Schematic as SVG + PNG + PDF in `final/hardware/images/` (labels fixed, rendered from the KiCad file, all 24 nets checked)
- [ ] Polar plot from `capture.py` of a real scene, next to a photo of that scene
- [ ] Uncomment the remaining image lines in the READMEs

## Code
- [ ] Final sketch(es) in `final/firmware/`, each version in its own folder
- [ ] Before/after code in final/firmware/README.md (your own rewrite vs v1)
- [ ] Code compiles from a clean Arduino IDE install

## GitHub
- [ ] Create a public repo, e.g. `arduino-ultrasonic-radar`
- [ ] Short "About" description: e.g. "Ultrasonic radar on an Arduino Uno R3 — HC-SR04 on a servo with LCD, LED/buzzer warnings and a serial radar display"
- [ ] Topics: `arduino`, `embedded-systems`, `ultrasonic-sensor`, `hc-sr04`, `servo`, `radar`, `electronics`, `kicad`
- [ ] Commit in stages rather than one big upload, so the history shows the progression
- [ ] Pin it on your GitHub profile
- [ ] Add to CV (Projects section, 2 lines + link) and LinkedIn (Projects + a post with `radar-demo.mp4` — LinkedIn plays MP4 video much better than GIFs)
