#include "SensorDevs.h"

#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

/* Ноты в мелодии */
int melody[] = {
  NOTE_C5, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5
};

int noteDurations[] = {
  4, 8, 8, 8, 4, 4, 4, 4, 4
};

void setup() {

  for (int i = 0; i < 9; i++) {
    int noteDuration = 1000 / noteDurations[i];

    tone(BUZ_PIN, melody[i], noteDuration); // Воспроизводим звук длительностью noteDuration
    delay(noteDuration * 1.30);
    noTone(BUZ_PIN);
  }
}

void loop() {
}