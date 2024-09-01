#include <Servo.h>

// Pin definitions
const int GAS_SENSOR_PIN = A1;
const int LDR_PIN = A0;
const int RELAY_PIN = 13;
const int SERVO_PIN = 7;
const int BUZZER_PIN = 8;
const int PIR_PIN = 9;
const int FAN_CONTROL_PIN = 10;
const int ULTRASONIC_PIN = 6;

// Threshold values
const int LIGHT_THRESHOLD = 500;
const int GAS_THRESHOLD = 400;
const int DISTANCE_THRESHOLD = 100;

// Other constants
const unsigned long MOTION_DELAY = 5000; // 5 seconds
const int BUZZER_FREQUENCY = 650;

Servo doorServo;

void setup() {
  Serial.begin(9600);
  
  pinMode(LDR_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(FAN_CONTROL_PIN, OUTPUT);
  
  doorServo.attach(SERVO_PIN, 500, 2500);
  
  Serial.println("Smart Home System Initialized");
}

long readUltrasonicDistance(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  return pulseIn(pin, HIGH);
}

void controlLight() {
  int lightLevel = analogRead(LDR_PIN);
  bool isLightOn = lightLevel > LIGHT_THRESHOLD;
  digitalWrite(RELAY_PIN, !isLightOn); // Relay is active LOW
  Serial.print("Light: ");
  Serial.print(isLightOn ? "OFF" : "ON");
  Serial.print(" (Level: ");
  Serial.print(lightLevel);
  Serial.println(")");
}

void controlFan() {
  bool motionDetected = digitalRead(PIR_PIN);
  digitalWrite(FAN_CONTROL_PIN, motionDetected);
  Serial.print("Fan: ");
  Serial.println(motionDetected ? "ON (Motion Detected)" : "OFF (No Motion)");
  if (motionDetected) {
    delay(MOTION_DELAY);
  }
}

void checkGasLevel() {
  int gasLevel = analogRead(GAS_SENSOR_PIN);
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);
  if (gasLevel > GAS_THRESHOLD) {
    tone(BUZZER_PIN, BUZZER_FREQUENCY, 300);
    Serial.println("WARNING: High Gas Level Detected!");
  }
}

void controlDoor() {
  float distance = 0.01723 * readUltrasonicDistance(ULTRASONIC_PIN);
  bool isDoorOpen = distance < DISTANCE_THRESHOLD;
  doorServo.write(isDoorOpen ? 90 : 0);
  Serial.print("Door: ");
  Serial.print(isDoorOpen ? "Open" : "Closed");
  Serial.print(" (Distance: ");
  Serial.print(distance);
  Serial.println(" cm)");
}

void loop() {
  Serial.println("\n--- Smart Home Status Update ---");
  controlLight();
  controlFan();
  checkGasLevel();
  controlDoor();
  Serial.println("--------------------------------");
  delay(1000); // Update every second
}
