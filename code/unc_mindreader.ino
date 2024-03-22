#include <FastLED.h>
#include <string.h>

#define LED_PIN1 7
#define LED_PIN2 8
#define buttonPin 3
#define buttonPin2 4
#define NUM_LEDS 60
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
int buttonState = 1;
int win_cutoff = 60;
int buttonState2 = 1;
int lastButtonState = 0;
int correct = 0;
int incorrect = 0;
int min_buffer_size = 1; //must be at least one
int max_buffer_size = 8;
String history = "";
void setup() {
  FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds1, NUM_LEDS);
  pinMode(buttonPin, INPUT);
  FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds2, NUM_LEDS);
  pinMode(buttonPin2, INPUT);
  Serial.begin(9600);
  correct = 0;
  incorrect = 0;
}
double get_votes(int n) {
  return -.4 * pow((n - 3), 2) + 5;
}
int submitGuess(int guess) {
  int votes_0 = 0;
  int votes_1 = 0;
  for (int n = min_buffer_size - 1; n < max_buffer_size; n++) {
    //get last n characters that occurred
    String last_n = "";
    if (n != 0) {
      last_n = history.substring(history.length() - n, history.length());
    }
    //count occurences of each pattern; do nothing in case of tie

    double zero = occurrences(history, last_n + "0", true);
    double one = occurrences(history, last_n + "1", true);
    if (zero > one) {
      votes_0 = votes_0 + zero / (zero + one + 0.0) * get_votes(n);

    }
    else if (one > zero) {
      votes_1 = votes_1 + one / (zero + one + 0.0) * get_votes(n);
    }
  }
  int prediction;
  //pick the value with the most votes
  if (votes_0 > votes_1) {
    prediction = 0;
  }
  else if (votes_0 < votes_1) {
    prediction = 1;
  }
  else {
    prediction = random(0, 2);
  }

  return prediction;
}
int occurrences(String string, String subString, bool allowOverlapping) {
  string = string + "";
  subString = subString + "";
  if (subString.length() <= 0) {
    return string.length() + 1;
  }
  int n = 0;
  int pos = 0;
  int step = 1;
  if (!allowOverlapping) {
    step = subString.length();
  }
  while (true) {
    pos = string.indexOf(subString, pos);
    if (pos >= 0) {
      n++;
      pos = pos + step;
    }
    else break;
  }
  return n;

}
//reset code and add victory loop, depending on who wins
void reset(int id) {
  history = "";
  correct = 0;
  incorrect = 0;
  int victory_loops = 1;
  if (id == 0) {
    for (int j = 0; j < victory_loops; j++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds1[i] = CRGB(75, 156, 211);
        leds2[i] = CRGB(75, 156, 211);
        FastLED.show();
        delay(50);
      }
    }
  }
  else {
    for (int j = 0; j < victory_loops; j++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds1[i] = CRGB(255, 145, 0);
        leds2[i] = CRGB(255, 145, 0);
        FastLED.show();
        delay(50);
      }
    }
  }

}
void loop() {
  int buttonState = digitalRead(buttonPin);
  int buttonState2 = digitalRead(buttonPin2);
  int prediction = submitGuess(0);
  Serial.println(prediction);
  Serial.println(history);

  if (buttonState == 0 && buttonState2 == 1) {
    int guess = 0;
    if (prediction == guess) {
      correct = correct + 1;
    }   else {
      incorrect = incorrect + 1;
    }

    history = history + String(guess);
  }
  if (buttonState == 1 && buttonState2 == 0) {
    int guess = 1;
    if (prediction == guess) {
      correct = correct + 1;
    }   else {
      incorrect = incorrect + 1;
    }

    history = history + String(guess);
  }

  for (int i = 0; i <= correct - 1; i++) {
    leds1[i] = CRGB(75, 156, 211);
    FastLED.show();
  }
  for (int i = 0; i <= incorrect - 1; i++) {
    leds2[i] = CRGB(255, 145, 0);
    FastLED.show();
  }
  for (int i = correct; i <= NUM_LEDS - 1; i++) {
    leds1[i] = CRGB(0, 0, 0);
    FastLED.show();
  }
  for (int i = incorrect; i <= NUM_LEDS - 1; i++) {
    leds2[i] = CRGB(0, 0, 0);
    FastLED.show();
  }
  if (correct == win_cutoff) {
    delay(1000);
    reset(0);
  }
  else if (incorrect == win_cutoff) {
    delay(3000);
    reset(1);
  }

  delay(10);
}
