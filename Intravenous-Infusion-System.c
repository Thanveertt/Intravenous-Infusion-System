//Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL3gxtgl3ax"
#define BLYNK_TEMPLATE_NAME "Drip Level"
#define BLYNK_AUTH_TOKEN "hhDSKNxbTLcEmw9KIiRRFwCvRrr2i64x"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "EDUREKA"; // Enter your WIFI name
char pass[] = "Edureka@123456";       // Enter your WIFI password

// Pin definitions
int level1=D6;  // NPN transistor connected to D0
int level2=D7;
int level3=D8;


const int servoPin = D5;
const int forwardButton = D3;
const int reverseButton = D4;
int currentAngle = 0;  // Start at 90° (mid-position)

// Pulse width for 0° = 500 µs, 180° = 2500 µs
const int minPulse = 500;
const int maxPulse = 2500;

// Convert degrees to pulse width
int angleToPulse(int angle) 
{
  return map(angle, 0, 30, minPulse, maxPulse);
}


void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set pin mode
  pinMode(level1, INPUT_PULLUP);
  pinMode(level2, INPUT_PULLUP);
  pinMode(level3, INPUT_PULLUP);

  pinMode(servoPin, OUTPUT);
  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(reverseButton, INPUT_PULLUP);
  moveServo(currentAngle);  // Move to initial position
  
  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  // Run Blynk
  Blynk.run();
  
  // Read button state directly from transistor pin
  int drip1 = !digitalRead(level1);
  int drip2 = !digitalRead(level2);
  int drip3 = !digitalRead(level3);
  
  // Send button state to Blynk for monitoring
  Blynk.virtualWrite(V0, drip1);
  Blynk.virtualWrite(V1, drip2);
  Blynk.virtualWrite(V2, drip3);
  
  // Debug info
  Serial.print("Drip Level 1: ");
  Serial.println(drip1);

  Serial.print("Drip Level 2: ");
  Serial.println(drip2);

  Serial.print("Drip Level 3: ");
  Serial.println(drip3);


  if (digitalRead(forwardButton) == LOW && currentAngle < 30 ) 
  {
    currentAngle++;  // Increase angle by 1 degree
    moveServo(currentAngle);
    delay(30);  // Debounce to avoid multiple triggers
  } 
  else if (digitalRead(reverseButton) == LOW && currentAngle > 0) 
  {
    currentAngle--;  // Decrease angle by 1 degree
    moveServo(currentAngle);
    delay(30);  // Debounce to avoid multiple triggers
  }
  
  delay(100); // Small delay to avoid bouncing
}


void moveServo(int angle)
{
  int pulseWidth = angleToPulse(angle);
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(servoPin, LOW);
  delay(20);  // 20ms frame rate for servo control
}
