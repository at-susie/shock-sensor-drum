#include <Control_Surface.h>

USBMIDI_Interface midi;

// ---- Your 4 trigger pins ----
const uint8_t TRIG_PINS[4] = {1, 2, 3, 4};     // keep your current wiring
uint8_t lastTrig[4] = {1, 1, 1, 1};            // INPUT_PULLUP => idle HIGH

// ---- KY-040 pins ----
const uint8_t ENC_CLK = 6;
const uint8_t ENC_DT  = 7;
const uint8_t ENC_SW  = 8;   // optional, can remove if unused

// ---- Current assignment (note per trigger) ----
uint8_t assignedNotes[4] = {41, 36, 37, 50};   // initial mapping

// ---- Pool of possible notes to randomize from ----
// Add as many as you like:
const uint8_t NOTE_POOL[] = {
  36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
  46, 47, 48, 49, 50, 51
};
const uint8_t NOTE_POOL_LEN = sizeof(NOTE_POOL) / sizeof(NOTE_POOL[0]);

// ---- Encoder state ----
int lastCLK = HIGH;

// Randomize: pick 4 notes from pool (can be duplicates; see below to avoid duplicates)
void randomizeAssignments() {
  for (int i = 0; i < 4; i++) {
    assignedNotes[i] = NOTE_POOL[random(NOTE_POOL_LEN)];
  }

  // Debug
  Serial.print("New assignment: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(assignedNotes[i]);
    Serial.print(i < 3 ? ", " : "\n");
  }
}

void setup() {
  Serial.begin(115200);
  Control_Surface.begin();

  // Triggers use pullups, active LOW
  for (int i = 0; i < 4; i++) {
    pinMode(TRIG_PINS[i], INPUT_PULLUP);
  }

  // Encoder pins
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  // Seed randomness (simple but ok for this use)
  randomSeed(analogRead(A0));

  randomizeAssignments();
}

void loop() {
  Control_Surface.loop();

  // ---- Read triggers and send MIDI note on press ----
  for (int i = 0; i < 4; i++) {
    uint8_t cur = digitalRead(TRIG_PINS[i]);

    if (cur != lastTrig[i]) {
      lastTrig[i] = cur;

      if (cur == LOW) {
        // Press => Note On
        midi.sendNoteOn({assignedNotes[i], CHANNEL_1}, 127);
      } else {
        // Release => Note Off
        midi.sendNoteOff({assignedNotes[i], CHANNEL_1}, 0);
      }
    }
  }

  // ---- KY-040 rotation => randomize assignments ----
  int clk = digitalRead(ENC_CLK);
  if (clk != lastCLK) {
    // On a CLK edge, check DT to know direction (optional)
    int dt = digitalRead(ENC_DT);

    // You said: rotating => randomized (direction not important)
    // So any “tick” triggers randomize:
    if (clk == LOW) {
      randomizeAssignments();
    }

    lastCLK = clk;
  }

  // ---- Optional: push button to randomize too ----
  static int lastSW = HIGH;
  int sw = digitalRead(ENC_SW);
  if (sw != lastSW) {
    lastSW = sw;
    if (sw == LOW) randomizeAssignments();
  }
}
