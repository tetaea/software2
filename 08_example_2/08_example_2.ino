// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 100      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)
#define DIST_CENTER 200.0 // ideal distance for maximum brightness

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficient to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float distance;

  // wait until next sampling time
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  if ((distance == 0.0) || (distance > _DIST_MAX) || (distance < _DIST_MIN)) {
      analogWrite(PIN_LED, 255);       // LED OFF (fully ON)
  } else {    // Distance within range
      int brightness = calculate_brightness(distance);  // calculate brightness
      analogWrite(PIN_LED, brightness); // set LED brightness
  }

  // output the distance to the serial port
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(", distance:");  Serial.print(distance);
  Serial.print(", Max:");       Serial.print(_DIST_MAX);
  Serial.println("");
  
  delay(50); // Assume that it takes 50ms to do something.
  
  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}

// calculate brightness based on distance
int calculate_brightness(float distance) {
  float brightness;

  if (distance <= _DIST_MIN || distance >= _DIST_MAX) {
    brightness = 255; // Fully ON (LED OFF)
  } else if (distance == DIST_CENTER) {
    brightness = 0; // Max brightness (LED fully ON)
  } else {
    float diff = abs(distance - DIST_CENTER);
    brightness = (255 * (diff / (DIST_CENTER - _DIST_MIN)));  // scale brightness
    if (brightness > 255) brightness = 255;  // Cap brightness at 255
  }

  return (int)brightness;
}
