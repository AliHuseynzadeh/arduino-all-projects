#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#define LED_PIN    27
#define LED_PIN2    9
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 720
#define LED_COUNT2 180
#define LED_COUNT3 180

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(LED_COUNT3, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(9600);
//  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//  strip.show();            // Turn OFF all pixels ASAP
//  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
//  
//  strip2.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//  strip2.show();            // Turn OFF all pixels ASAP
//  strip2.setBrightness(255);
//  
//  strip3.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//  strip3.show();            // Turn OFF all pixels ASAP
//  strip3.setBrightness(255);
rainbow(255);
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  // Fill along the length of the strip in various colors...
  colorWipe2(strip2.Color(255,   0,   0), 50); // Red
  //colorWipe(strip.Color(  0, 255,   0), 50); // Green
  colorWipe3(strip3.Color(  0,   0, 255), 50); // Blue


  //rainbow(10);
  //theaterChaseRainbow(50);
 
  
}

void colorWipe(uint32_t color, int wait) {
  strip.setPixelColor(10, color);
}

void colorWipe2(uint32_t color, int wait) {
 strip2.setPixelColor(20, color);
}

void colorWipe3(uint32_t color, int wait) {
 for(int i=80; i<120; i++) { // For each pixel in strip...
    Serial.print(strip3.numPixels());
    strip3.setPixelColor(i, strip3.Color(  0,   0, 255));         //  Set pixel's color (in RAM)
    strip3.show();                          //  Update strip to match
    delay(20);                           //  Pause for a moment
  }

  for(int i=60; i<80; i++) { // For each pixel in strip...
    Serial.print(strip3.numPixels());
    strip3.setPixelColor(i, strip3.Color(  255,   0, 0));         //  Set pixel's color (in RAM)
    strip3.show();                          //  Update strip to match
    delay(20);                           //  Pause for a moment
  }

  for(int i=20; i<60; i++) { // For each pixel in strip...
    Serial.print(strip3.numPixels());
    strip3.setPixelColor(i, strip3.Color(  0,   255, 0));         //  Set pixel's color (in RAM)
    strip3.show();                          //  Update strip to match
    delay(20);                           //  Pause for a moment
  }

  for(int i=0; i<80; i++) { // For each pixel in strip...
    Serial.print(strip3.numPixels());
    strip3.setPixelColor(i, strip3.Color(  255,   0, 0));         //  Set pixel's color (in RAM)
    strip3.show();                          //  Update strip to match
    delay(20);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait); 
       strip2.show(); // Update strip with new contents
      delay(wait);// Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(),i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      
      strip2.setPixelColor(i, strip2.gamma32(strip2.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);
     strip2.show(); // Update strip with new contents
    delay(wait);// Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();
      strip2.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(), c<strip2.numPixels();c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); 
        uint32_t color2 = strip2.gamma32(strip2.ColorHSV(hue)); // hue -> RGB
        strip2.setPixelColor(c, color);// Set pixel 'c' to value 'color'
      }
      strip.show();  
       strip2.show();  // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
