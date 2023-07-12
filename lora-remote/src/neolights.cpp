#ifdef OUTPUT_NEOPIXEL
#include <Adafruit_NeoPixel.h>
#include <config.h>
#include <neolights.h>

// Configured in "config.h"
// Parameter 1 = Number of pixels in strip
// Parameter 2 = Arduino pin number
// Parameter 3 = Pixel type flags, add together as needed:
Adafruit_NeoPixel pixels(NeoPixelCount, NeoPixelPin, NeoPixelFlags);


int r, g, b = 0;
int target_r,target_b,target_g = 0;
int resting_state_g, resting_state_r, resting_state_b = 0;

bool currentlyAlerting = false;

int override_target_r,override_target_g,override_target_b = 0;


// After the new Powergrid code is downloaded, this function is called to change the NeoPixel updates
void flashNeoPixels() {
    if (currentlyAlerting) {
        target_r = 255;
        target_g = 0;
        target_b = 0;
    } else {
        target_r = 0;
        target_g = 255;
        target_b = 0;
    }

}

void alertLight(bool state) {
    if (state) {
        pixels.setPixelColor(NeoPixelCount-1, pixels.Color(255, 255, 0));
    } else {
        pixels.setPixelColor(NeoPixelCount-1, pixels.Color(0, 0, 0));
    }
    pixels.show();
}

// Runs once on startup, before attempting to connect to wifi
void setupNeoPixels() {
    // INITIALIZE NeoPixel strip object
    pixels.begin();
    delay(500);
    pixels.clear();  // Set all pixel colors to 'off'
    pixels.show();
    setAlerting(false);
    
}


void showNeoPixelsError() {
    pixels.fill(pixels.Color(255, 0, 255), 0, NeoPixelCount);
	pixels.show();
}

void setAlerting(bool state) {
    currentlyAlerting = state;
    if (state) {
        alertLight(true);
        resting_state_r = 10;
        resting_state_g = 0;
        resting_state_b = 0;

    } else {
        alertLight(false);
        resting_state_r = 0;
        resting_state_g = 10;
        resting_state_b = 0;
    }
}

void animateNeoPixel() {
    if (r < target_r) {r++;} else if (r > target_r) {r--;}
    if (g < target_g) {g++;} else if (g > target_g) {g--;}
    if (b < target_b) {b++;} else if (b > target_b) {b--;}

    if (r == target_r) {target_r = resting_state_r;}
    if (g == target_g) {target_g = resting_state_g;}
    if (b == target_b) {target_b = resting_state_b;}

    if (r!=target_r || g!=target_g || b!=target_b) {
        pixels.fill(pixels.Color(r, g, b), 0, NeoPixelCount-1);
    }
    pixels.show();
}
#endif