#include <FastLED.h>
#define NUM_LEDS 8
#define DATA_PIN 7
#define CLOCK_PIN 5
#define DARK_THRESHHOLD 30
#define ANIM_FRAME 10
#define ANIM_DURATION 2000
#define LIGHT_DURATION 10000

int pir = 0;
int ldr = 0;
int white_bright = 255; // default brightness for white (0-255)
int buzz_disable = 1;   // change to 1 to switch off initial buzzer

int colors[][3] = {
    {0, 255, 0},   // green
    {0, 255, 255}, // cyan
    {255, 0, 0},   // red
    {0, 0, 255},   // blue
    {255, 255, 0},
    {255, 0, 255}
};
int current_color = 0;
int fade_n = ANIM_DURATION / ANIM_FRAME;

CRGBArray<NUM_LEDS> leds;

void setup()
{
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  FastLED.setBrightness(white_bright); // Global Brightness setting max 255

  pinMode(12, INPUT);  // PIR motion sensor read port
  pinMode(2, OUTPUT);  // buzzer port
  pinMode(13, OUTPUT); // LED port
  if (buzz_disable == 0)
  {
    digitalWrite(2, HIGH); // buzzer ON, if not disabled
  }
  digitalWrite(13, HIGH); // PIR LED ON
  delay(1000);
  digitalWrite(2, LOW);  // buzzer OFF
  digitalWrite(13, LOW); // PIR LED OFF
}

void loop()
{
  FastLED.clear();
  // off
  for (int i = 0; i < NUM_LEDS; i++)
  {                          // iterate through the RGB LEDs
    leds[i].setRGB(0, 0, 0); // set default colour or off
  }
  FastLED.show();
  
  delay(500); // Delay so when dark the LED will flash on for 0.1s
  
  int color_r = colors[current_color][0];
  int color_g = colors[current_color][1];
  int color_b = colors[current_color][2];
  current_color = (current_color + 1) % 6;

    
  // on
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(color_r, color_g, color_b);
  }
  FastLED.show();
  delay(100);
}
