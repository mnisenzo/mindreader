#include <FastLED.h>

#define LED_PIN1 7
#define buttonPin 2
#define NUM_LEDS 60
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
int buttonState = 0;
int lastButtonState = 0;
int correct = 0;
int incorrect = 0;
void setup() {
  FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds1, NUM_LEDS);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  correct = 0;
  incorrect = 0;
}

void loop() {
  buttonState = digitalRead(buttonPin);

  Serial.print(correct);
  if (correct + incorrect == NUM_LEDS){
    correct = 0;
    incorrect = 0;
  } else if (lastButtonState != buttonState && buttonState == HIGH){
    correct = correct + 1;
    
    for(int i = 0; i<= correct-1; i++){
      leds1[i] = CRGB(128, 0, 128);
      FastLED.show();
    }
    delay(50);
  } else {
      for(int i = 0; i<= correct-1; i++){
        leds1[i] = CRGB(255, 0, 0);
        FastLED.show();
      }
      
  }
   for(int i = correct; i<= NUM_LEDS-1; i++){
      leds1[i] = CRGB(0, 0, 0);
      FastLED.show();
    }
  lastButtonState = buttonState;
  delay(20);
}
