#!/usr/bin/env python3
"""
Capture radar readings from the Arduino over serial and save them.

Usage:
    python3 capture.py <serial port> [sweeps]

    e.g. python3 capture.py COM5 2              (Windows)
         python3 capture.py /dev/ttyACM0 2      (Linux)
         python3 capture.py /dev/cu.usbmodem1101 2   (Mac)

Reads "angle,distance." messages, stops after the given number of sweeps
(default 1), then writes:
    sweep_<timestamp>.csv   time_s, angle_deg, distance_cm
    sweep_<timestamp>.png   polar plot of the detected points

Needs: pip install pyserial matplotlib
"""

import csv
import math
import sys
import time
from datetime import datetime

import serial
import matplotlib.pyplot as plt

BAUD = 9600


def read_messages(port):
    """Yield (angle, distance) tuples parsed from the 'angle,distance.' stream."""
    buffer = ""
    while True:
        chunk = port.read(64).decode("ascii", errors="ignore")
        if not chunk:
            continue
        buffer += chunk
        while "." in buffer:
            message, buffer = buffer.split(".", 1)
            message = message.strip()
            if "," not in message:
                continue
            try:
                angle_text, dist_text = message.split(",", 1)
                yield int(angle_text), int(dist_text)
            except ValueError:
                continue  # partial or corrupted message at start-up


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    port_name = sys.argv[1]
    sweeps_wanted = int(sys.argv[2]) if len(sys.argv) > 2 else 1

    rows = []
    sweeps_done = 0
    last_angle = None
    direction = None

    with serial.Serial(port_name, BAUD, timeout=1) as port:
        time.sleep(2)          # the Uno resets when the port opens
        port.reset_input_buffer()
        start = time.time()
        print(f"Capturing {sweeps_wanted} sweep(s) from {port_name}...")

        for angle, distance in read_messages(port):
            t = time.time() - start
            rows.append((round(t, 3), angle, distance))

            # Count a sweep each time the direction reverses
            if last_angle is not None and angle != last_angle:
                new_direction = 1 if angle > last_angle else -1
                if direction is not None and new_direction != direction:
                    sweeps_done += 1
                    print(f"  sweep {sweeps_done} done at {t:.2f} s")
                    if sweeps_done >= sweeps_wanted:
                        break
                direction = new_direction
            last_angle = angle

    stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    csv_name = f"sweep_{stamp}.csv"
    png_name = f"sweep_{stamp}.png"

    with open(csv_name, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["time_s", "angle_deg", "distance_cm"])
        writer.writerows(rows)

    # Polar plot of the points where something was detected (distance > 0)
    hits = [(a, d) for _, a, d in rows if d > 0]
    fig = plt.figure(figsize=(7, 4.5))
    ax = fig.add_subplot(projection="polar")
    ax.set_thetamin(0)
    ax.set_thetamax(180)
    if hits:
        ax.scatter([math.radians(a) for a, _ in hits], [d for _, d in hits], s=8)
    ax.set_rmax(200)
    ax.set_title("Detected objects (cm)")
    fig.tight_layout()
    fig.savefig(png_name, dpi=150)

    print(f"Saved {len(rows)} readings to {csv_name} and the plot to {png_name}")


if __name__ == "__main__":
    main()
