#define NUM_LEDS 11
#define PHASE_DURATION 5000UL  // 5 seconds per phase x 5 phases inside megaEffect = 25 seconds

int ledPins[NUM_LEDS] = {2, 4, 5, 18, 19, 21, 22, 23, 15, 26, 13};  // D2, D4, D5, D18, D19, D21, D22, D23, D15, D26, D13

void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  randomSeed(millis());
}

void loop() {
  chaseEffect();           // ~1.1s
  delay(500);

  buildLineEffect();       // ~15s
  delay(1000);

  megaEffect();            // ~25s
  delay(1000);

  centerBurstEffect();     // ~4s
  delay(1000);

  binaryCounterEffect();   // ~8s
  delay(1000);
  // Total cycle: roughly 58-60 seconds
}

// Effect 1: original simple chase
void chaseEffect() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

// Effect 2: build up the line in BOTH directions
void buildLineEffect() {
  for (int stand = 0; stand < NUM_LEDS; stand++) {
    for (int run = NUM_LEDS - 1; run >= stand; run--) {
      digitalWrite(ledPins[run], HIGH);
      delay(100);
      if (run != stand) digitalWrite(ledPins[run], LOW);
    }
  }
  delay(700);
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
  delay(300);

  for (int stand = NUM_LEDS - 1; stand >= 0; stand--) {
    for (int run = 0; run <= stand; run++) {
      digitalWrite(ledPins[run], HIGH);
      delay(100);
      if (run != stand) digitalWrite(ledPins[run], LOW);
    }
  }
  delay(700);
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
}

// Effect 3: ~25 second combo — fast comet bounce, slow pulse,
// fast random twinkle, accelerating chase, strobe finale
void megaEffect() {
  unsigned long phaseStart;

  phaseStart = millis();
  int pos = 0;
  int direction = 1;
  while (millis() - phaseStart < PHASE_DURATION) {
    digitalWrite(ledPins[pos], HIGH);
    int trail = pos - direction;
    if (trail >= 0 && trail < NUM_LEDS) digitalWrite(ledPins[trail], HIGH);
    delay(40);
    digitalWrite(ledPins[pos], LOW);
    if (trail >= 0 && trail < NUM_LEDS) digitalWrite(ledPins[trail], LOW);
    pos += direction;
    if (pos >= NUM_LEDS - 1 || pos <= 0) direction *= -1;
  }
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);

  phaseStart = millis();
  while (millis() - phaseStart < PHASE_DURATION) {
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], HIGH);
    delay(600);
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
    delay(600);
  }

  phaseStart = millis();
  while (millis() - phaseStart < PHASE_DURATION) {
    int randomLED = random(0, NUM_LEDS);
    digitalWrite(ledPins[randomLED], HIGH);
    delay(random(20, 80));
    digitalWrite(ledPins[randomLED], LOW);
  }

  phaseStart = millis();
  int speedDelay = 300;
  while (millis() - phaseStart < PHASE_DURATION) {
    for (int i = 0; i < NUM_LEDS; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(speedDelay);
      digitalWrite(ledPins[i], LOW);
      if (speedDelay > 30) speedDelay -= 5;
      if (millis() - phaseStart >= PHASE_DURATION) break;
    }
  }

  phaseStart = millis();
  while (millis() - phaseStart < PHASE_DURATION / 2) {
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], HIGH);
    delay(50);
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
    delay(50);
  }
  for (int b = 0; b < 3; b++) {
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], HIGH);
    delay(500);
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
    delay(500);
  }
}

// Effect 4: Center burst — LEDs radiate outward from the middle,
// hold fully lit, then implode back inward to the center
void centerBurstEffect() {
  int center = NUM_LEDS / 2;

  for (int rep = 0; rep < 3; rep++) {
    for (int offset = 0; offset <= center; offset++) {
      int left = center - offset;
      int right = center + offset;
      if (left >= 0) digitalWrite(ledPins[left], HIGH);
      if (right < NUM_LEDS) digitalWrite(ledPins[right], HIGH);
      delay(80);
    }
    delay(200);

    for (int offset = center; offset >= 0; offset--) {
      int left = center - offset;
      int right = center + offset;
      if (left >= 0) digitalWrite(ledPins[left], LOW);
      if (right < NUM_LEDS) digitalWrite(ledPins[right], LOW);
      delay(80);
    }
    delay(200);
  }
}

// Effect 5: Binary counter — each LED is one bit, counting upward
void binaryCounterEffect() {
  unsigned long duration = 8000;
  unsigned long start = millis();
  long count = 0;

  while (millis() - start < duration) {
    for (int i = 0; i < NUM_LEDS; i++) {
      digitalWrite(ledPins[i], bitRead(count, i) ? HIGH : LOW);
    }
    delay(150);
    count++;
    if (count > (1L << NUM_LEDS) - 1) count = 0;
  }

  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW);
}
