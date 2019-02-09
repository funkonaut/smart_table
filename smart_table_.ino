#include <FirebaseArduino.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"

#include <ESP8266WiFi.h>

// How many leds in your strip?
#define NUM_LEDS 152 

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
static uint8_t hue = 0;
// Set up server
#define FIREBASE_HOST "smart-table-fd493.firebaseio.com"
#define FIREBASE_AUTH "Ztm2S6MnnGucBPpUbMYRaKUR2K28T2EeKoB4kR2k"
#define WIFI_SSID "TheChickenRanch"
#define WIFI_PASSWORD "PleaseL3t0s1n"

void setup() { 
  pinMode (DATA_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
  
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() {
  hue = Firebase.getInt("Hue");
  if (Firebase.failed()) {
      Serial.println("Firebase get failed attempting reconnect");
      Serial.println(Firebase.error());
      if (WiFi.status() != WL_CONNECTED) {
          WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
           while (WiFi.status() != WL_CONNECTED)
           {
             delay(500);
             Serial.print(".");
           }
          Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      }
      return;
  }
   
 
  Serial.println(hue);
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}
