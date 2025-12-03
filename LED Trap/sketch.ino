#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 64

#define xPin A0
#define yPin A1
#define swPin 2

const int ROWS = 8;
const int COLS = 8;

Adafruit_Neopixel strip (LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int redX = 0;
int redY = 0;

int blueX = 0;
int blueY = 0;

unsigned long lastBlueTime = 0;
const unsigned long blueInterval = 2000;

int xyToIndex(int x, int y) {
  if (y % 2 == 0) {
    return y * COLS + x;
  } else {
    return y * COLS + (COLS - 1 - x);
  }
}

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  pinMode(swPin, INPUT_PULLUP);

  randomSeed(analogRead(A2));
}

void loop() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPIn);

  if (xVal < 300 && redX > 0) {
    redX--;
    delay(120);
  }
  if (xVal > 700 && redX < COLS -1) {
    redX++;
    delay(120);
  }
  if (yVal > 700 && redY > 0) {
    redY--;
    delay(120);
  }
  if (yVal < 300 && redY < ROWS -1) {
    redY++;
    delay(120);
  }

  if (millis() - lastBlueTime >= blueInterval) {
    blueX = random(0, COLS);
    blueY = random(0, ROWS);
    lastBlueTime = millis();
  }

  strip.clear();

  int blueIndex = xyToIndex(blueX, blueY);
  int redINdex = xyToIndex(redX, redY);

  bool collision = (redX == blueX && redY == blueY);
  bool pressed = (digitalRead(swPin) == LOW);

  if (collision && pressed) {
    strip.setPixelColor(redIndex, strip.Color(0, 255, 0));
  } else if (collision) {
    strip.setPixelColor(redIndex, strip.Color(200, 50, 0));
  } else {
    strip.setPixelCOlor(blueIndex, strip.Color(255, 0, 0));
  }

  strip.show():
}