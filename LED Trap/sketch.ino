// Adafruit Neopixel library for WS2818 RGB LEDs
#include <Adafruit_NeoPixel.h>

// RGB LED pin and count
#define LED_PIN   6
#define LED_COUNT 64

// Joystick pins
#define xPin A0
#define yPin A1
#define swPin 2

// Number of rows and columns
const int ROWS = 8;
const int COLS = 8;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// "Cat" (player, blue LED) position
int blueX = 0;
int blueY = 0;

// "Mouse" (red LED) position
int redX = 0;
int redY = 0;

// Speed interval for red LED
unsigned long redInterval = 2000;
unsigned long lastRedTime = 0;

// Joystick calibration
int centerX = 512;
int centerY = 512;
int deadzone = 80;

// Converting X and Y to LED index
int xyToIndex(int x, int y) {
  if (y % 2 == 0) return y * COLS + x;
  return y * COLS + (COLS - 1 - x);
}

void setup() {
  Serial.begin(115200);

  // LED strip power and brightness
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  // Joystick inputs
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(swPin, INPUT_PULLUP);
  
  // For random LED positions
  randomSeed(analogRead(A2));

  // Calibrating joystick center
  long joyX = 0, joyY = 0;
  for (int i = 0; i < 40; i++) {
    joyX += analogRead(xPin);
    joyY += analogRead(yPin);
    delay(5);
  }
  centerX = joyX / 40;
  centerY = joyY / 40;
}

void readSerialInput() {
  // Level data between Arduino and website
  if (!Serial.available()) return;
  String s = Serial.readStringUntil('\n');
  s.trim();
  int levelReceived = s.toInt();

  // Increase speed of red LED each level (max 300 ms)
  if (levelReceived >= 1 && levelReceived <= 20) {
    redInterval = max(300, 2000 - (levelReceived - 1) * 150);
    Serial.print(levelReceived);
    Serial.println(redInterval);
  }
}

void loop() {
  readSerialInput();

  // Joystick values
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  bool pressed = (digitalRead(swPin) == LOW);

  int lowX  = centerX - deadzone;
  int highX = centerX + deadzone;
  int lowY  = centerY - deadzone;
  int highY = centerY + deadzone;

  // Player movement control
  static unsigned long lastMove = 0;
  if (millis() - lastMove > 70) {
    if (xVal < lowX && blueX > 0) blueX--;
    if (xVal > highX && blueX < COLS - 1) blueX++;
    if (yVal > highY && blueY > 0) blueY--;
    if (yVal < lowY && blueY < ROWS - 1) blueY++;
    lastMove = millis();
  }

  // Move red LED to random position at interval speed
  if (millis() - lastRedTime >= redInterval) {
    redX = random(0, COLS);
    redY = random(0, ROWS);
    lastRedTime = millis();
  }

  strip.clear();

  // Red and blue index
  int redIndex  = xyToIndex(redX,  redY);
  int blueIndex = xyToIndex(blueX, blueY);

  // Collision if red and blue LEDs are in the same position
  bool collision = (redX == blueX && redY == blueY);

  // Collision colors
  if (collision && pressed) {
    // If there's collision and sw is pressed, green LED
    strip.setPixelColor(blueIndex, strip.Color(0, 255, 0));
    Serial.println("caught");
  }
  else if (collision) {
    // If there's collision, yellow LED
    strip.setPixelColor(blueIndex, strip.Color(255, 255, 0));
  }
  else {
    // Blue LED is player, red LED is "mouse"
    strip.setPixelColor(blueIndex, strip.Color(0, 0, 255));
    strip.setPixelColor(redIndex, strip.Color(255, 0, 0));
  }

  strip.show();
}