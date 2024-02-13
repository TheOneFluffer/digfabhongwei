#include <Adafruit_NeoPixel.h> 
 
// Define constants 
const int buttonPin = 2;  // Pin connected to the button 
const int ledPin = 4;    // Pin connected to the NeoPixel strip 
const int numPixels = 6; // Number of pixels in the strip (Rmb to change to 12 when actual project is done) 
const int modeMax = 4;   // Maximum number of modes (including reset mode) 
 
unsigned long currentMillis = 0; 
const unsigned long longPressTime = 20; // Adjust for desired hold time in milliseconds 
const int buttonReleased = 0; 
const int buttonHold = 1; 
int buttonState = buttonReleased; 
 
// Create a NeoPixel object 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, ledPin, NEO_GRB + NEO_KHZ800); 
 
int currentMode = 0;  // Keep track of the current mode 
 
void setup() { 
  Serial.begin(9600); 
  strip.begin();       // Initialize the NeoPixel strip 
  strip.show();        // Turn off all pixels initially
  strip.setBrightness(50);
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with internal pullup 
} 
 
void loop() { 
  if (digitalRead(buttonPin) == LOW) { // Button pressed 
    currentMillis = millis(); 
    buttonState = buttonHold; 
  } else if (buttonState == buttonHold && digitalRead(buttonPin) == HIGH) { 
    // Button released after hold 
    unsigned long holdTime = millis() - currentMillis; 
    if (holdTime >= longPressTime) { 
      currentMode = 0; // Turn off 
    } else { 
      // Short press behavior (cycle modes) 
      currentMode = (currentMode + 1) % (modeMax + 1); 
    } 
    buttonState = buttonReleased; 
    delay(15); // Debounce delay 
  } 
 
  switch (currentMode) { 
    case 0: // Off 
      strip.clear(); 
      break; 
    case 1: // Red 
      setAllPixels(strip.Color(150, 0, 0)); 
      break; 
    case 2: // Green 
      setAllPixels(strip.Color(0, 150, 0)); 
      break; 
    case 3: // Blue 
      setAllPixels(strip.Color(0, 0, 150)); 
      break; 
    case 4: // Rainbow 
      rainbowCycle(10);
      break; 
  }; 
  strip.show(); // Update the strip 
} 
 
// Helper functions (unchanged) 
void setAllPixels(uint32_t color) { 
  for (int i = 0; i < numPixels; i++) { 
    strip.setPixelColor(i, color); 
  } 
}

void rainbowCycle(int speedDelay) {
  unsigned long startTime = millis(); // Record the start time of the rainbow cycle
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(speedDelay);

    // Check if the hold time exceeds longPressTime, and exit the function if so
    if (millis() - startTime >= longPressTime) {
      return;
    }
  }
}
