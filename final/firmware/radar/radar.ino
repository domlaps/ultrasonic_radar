/*
  Ultrasonic Radar - Arduino Uno R3
  Version 1 (written with help, then learned line by line)

  Parts: HC-SR04, SG90 servo, LCD1602 (+10k pot), active buzzer,
         red/yellow/green LEDs (+220 ohm resistors)

  - Servo sweeps the sensor from 15 to 165 degrees and back
  - LCD shows the current angle and distance
  - LEDs: green = clear, yellow = object 20-50 cm, red = object under 20 cm
  - Buzzer: slow beeps for yellow, fast beeps for red
  - Sends "angle,distance." over serial for a radar screen on the laptop
*/

#include <Servo.h>
#include <LiquidCrystal.h>

// ---------- Pins ----------
const int TRIG_PIN   = 10;
const int ECHO_PIN   = 11;
const int SERVO_PIN  = 12;
const int BUZZER_PIN = 6;
const int GREEN_LED  = A0;   // analogue pins work as normal digital pins too
const int YELLOW_LED = A1;
const int RED_LED    = A2;

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 5, 4, 3, 2);
Servo radarServo;

// ---------- Settings ----------
const int MIN_ANGLE  = 15;    // keep away from the servo's end stops
const int MAX_ANGLE  = 165;
const int STEP_DELAY = 30;    // ms to wait at each degree (servo settles)

const int CLOSE_CM  = 20;     // red zone
const int MEDIUM_CM = 50;     // yellow zone
const unsigned long ECHO_TIMEOUT_US = 12000;  // about 2 m max range

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  radarServo.attach(SERVO_PIN);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Radar starting");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Sweep left to right, then right to left
  for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle++) {
    scanAt(angle);
  }
  for (int angle = MAX_ANGLE; angle >= MIN_ANGLE; angle--) {
    scanAt(angle);
  }
}

// Move to an angle, measure, and update every output
void scanAt(int angle) {
  radarServo.write(angle);
  delay(STEP_DELAY);

  int distance = readDistance();   // 0 means nothing detected

  updateWarnings(distance);
  updateLCD(angle, distance);

  // Format for the radar screen: angle,distance.
  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
}

// Returns distance in cm, or 0 if no echo came back in range
int readDistance() {
  // Send a 10 microsecond pulse on Trig
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure how long Echo stays HIGH (the sound's round trip time)
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    return 0;
  }

  // Speed of sound = 0.0343 cm/us, halved because the sound goes there and back
  return duration * 0.0343 / 2;
}

// LEDs and buzzer based on distance
void updateWarnings(int d) {
  bool close  = (d > 0 && d < CLOSE_CM);
  bool medium = (d >= CLOSE_CM && d < MEDIUM_CM);

  digitalWrite(RED_LED,    close);
  digitalWrite(YELLOW_LED, medium);
  digitalWrite(GREEN_LED,  !close && !medium);

  // Beep pattern using millis() so it never pauses the sweep
  unsigned long t = millis();
  if (close) {
    digitalWrite(BUZZER_PIN, (t % 150) < 75);    // fast beeping
  } else if (medium) {
    digitalWrite(BUZZER_PIN, (t % 500) < 75);    // slow beeping
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// Show angle and distance on the LCD
void updateLCD(int angle, int d) {
  lcd.setCursor(0, 0);
  lcd.print("Angle: ");
  lcd.print(angle);
  lcd.print((char)223);       // degree symbol on the LCD1602
  lcd.print("    ");          // clear leftover digits

  lcd.setCursor(0, 1);
  lcd.print("Dist: ");
  if (d == 0) {
    lcd.print("clear");
  } else {
    lcd.print(d);
    lcd.print(" cm");
  }
  lcd.print("    ");
}
