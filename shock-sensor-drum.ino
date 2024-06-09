#include <Control_Surface.h>

// Create an array of NoteButton instances
NoteButton buttons[] = {
   {2, {MIDI_Notes::C(4), CHANNEL_1}}, // Note C4 on MIDI channel 1
   // Add more buttons as needed
};
NoteButton drum1 {
  1,
  {41, CHANNEL_1}, 
  //{MIDI_Notes::C(4), CHANNEL_1}, // Note C4 on MIDI channel 1
};

NoteButton drum2 {
  2,
  {36, CHANNEL_1}, 
  //{MIDI_Notes::D(4), CHANNEL_1}, // Note D4 on MIDI channel 1
};

NoteButton drum3 {
  3,                       // Push button on pin 3
  {37, CHANNEL_1}, 
};

NoteButton drum4 {
  4,                       // Push button on pin 4
  {50, CHANNEL_1}, 
};

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi;

void setup() {
   // Initialize Control Surface library
   Control_Surface.begin();
   drum1.begin();
   drum2.begin();
   drum3.begin();
   drum4.begin();
}

void loop() {
   // Update Control Surface library
   Control_Surface.loop();
   drum1.update();
   drum2.update();
   drum3.update();
   drum4.update();
}
