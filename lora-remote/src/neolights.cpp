#ifdef OUTPUT_NEOPIXEL
#include <Adafruit_NeoPixel.h>
#include <config.h>
#include <neolights.h>

// Configured in "config.h"
// Parameter 1 = Number of pixels in strip
// Parameter 2 = Arduino pin number
// Parameter 3 = Pixel type flags, add together as needed:
Adafruit_NeoPixel pixels(NeoPixelCount, NeoPixelPin, NeoPixelFlags);

// After the new Powergrid code is downloaded, this function is called to change the NeoPixel updates
void updateNeoPixels(int r, int g, int b) {
    
    pixels.fill(pixels.Color(r, g, b), 0, NeoPixelCount);

    pixels.show();
}

// Runs once on startup, before attempting to connect to wifi
void setupNeoPixels() {
    // INITIALIZE NeoPixel strip object
    pixels.begin();
    delay(500);
    pixels.clear();  // Set all pixel colors to 'off'
    pixels.show();
}


void showNeoPixelsError() {
    pixels.fill(pixels.Color(255, 0, 255), 0, NeoPixelCount);
	pixels.show();
}
#endif