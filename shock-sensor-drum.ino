#include <Control_Surface.h>

USBMIDI_Interface midi;

// Pins: 1,2,3,4 (keep as-is if that's how you're wired today)
NoteButton drum1 {1, {41, CHANNEL_1}}; // Low Floor Tom (or whatever your instrument maps)
NoteButton drum2 {2, {36, CHANNEL_1}}; // Kick
NoteButton drum3 {3, {37, CHANNEL_1}}; // Side Stick
NoteButton drum4 {4, {50, CHANNEL_1}}; // High Tom

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
