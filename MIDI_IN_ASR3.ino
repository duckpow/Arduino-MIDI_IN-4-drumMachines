#include <MIDI.h> // midi library necessary

// PIN's for ASR's
int pins[] = {13,12,8,7};
int state[] = {LOW, LOW, LOW, LOW};
int leds[] = {11,10,9,6}; // hardware not yet implemented
// timer
long preMillis[] = {0,0,0,0}; //should proably be unsigned!!
long interval = 50;  // an unsigned int should suffice? faster with a long?
unsigned long currentMillis = 0;

// set state and timestamps in case of noteon
void noteOn(int note, int velocity) {
  // Clean velocity to light
  if (note < 4) {
    state[note] = HIGH;
    preMillis[note] = currentMillis;
    analogWrite(leds[note],velocity);
  }
}


void setup() {
  // init pins with loop;
  for(int i=0; i<4; i++){
    pinMode(pins[i], OUTPUT);
  }
  // init led pins
  for(int i=0; i<4; i++){
    pinMode(leds[i], OUTPUT);
  }
  MIDI.begin(1); // input channel is default value 1
}

void loop() {
  // timing
  currentMillis = millis();
  // Midi
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    switch(MIDI.getType()) {		// Get the type of the message we caught
      case NoteOn:
        noteOn(MIDI.getData1(), MIDI.getData2());
        break;
      default:
        break;
    }
  }
  // Loop trough all pins
  for(int i; i<4; i++){
    digitalWrite(pins[i],state[i]); // write the current state for each pin
    // change state of pin based on time delay
    if(state[i] == HIGH && (currentMillis - preMillis[i] > interval)){
      state[i] = LOW;
    }
  }
}
