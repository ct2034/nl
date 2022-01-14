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
    {0, 255, 255}, // cyan
    {255, 0, 0}    // red
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

  // DEMO
  randomSeed(analogRead(0));
  for (int t = 0; t < NUM_LEDS; t++)
  {
    for (int i = 0; i < t; i++)
    {
      leds[i].setRGB(random(0, 120), random(0, 120), random(0, 120));
    }
    FastLED.show();
    delay(ANIM_FRAME);
  }

  // DEBUG
  // Serial.begin(115200);
}

void loop()
{
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++)
  {                          // iterate through the RGB LEDs
    leds[i].setRGB(0, 0, 0); // set default colour or off
  }
  pir = digitalRead(12); // check PIR
  ldr = analogRead(7);   // read light sensor (0 = very dark, 1023 = very bright)
                         //      50-150 is usually a suitable threshold
  if (pir == 1)
  { // If motion detected
    //digitalWrite(13, HIGH);                // PIR LED ON
    delay(100); // Delay so when dark the LED will flash on for 0.1s
  }
  else
  {
    digitalWrite(13, LOW); // PIR LED OFF
  }
  if (pir == 1 && ldr <= DARK_THRESHHOLD)
  {
    digitalWrite(13, LOW); // Don't need PIR LED on now

    int color_r = colors[current_color][0];
    int color_g = colors[current_color][1];
    int color_b = colors[current_color][2];
    current_color = (current_color + 1) % 2;

    // fade to on
    for (int t = 0; t < fade_n; t++)
    {
      float brightness = (float)t / fade_n;
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setRGB(color_r * brightness, color_g * brightness, color_b * brightness);
      }
      FastLED.show();
      delay(ANIM_FRAME);
    }

    // on
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i].setRGB(color_r, color_g, color_b);
    }
    FastLED.show();
    delay(LIGHT_DURATION - ANIM_DURATION);

    // fade to off
    for (int t = 0; t < fade_n; t++)
    {
      float brightness = (float)(fade_n - t) / fade_n;
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setRGB(color_r * brightness, color_g * brightness, color_b * brightness);
      }
      FastLED.show();
      delay(ANIM_FRAME);
    }

    // off
    FastLED.clear(); // reset LEDs to 0,0,0
    FastLED.show();  // lights off
  }
  else
  {
    FastLED.show(); // display current LED settings
    delay(500);     // otherwise poll PIR sensor at 2 Hertz
  }
  // Serial.println(ldr);
}
