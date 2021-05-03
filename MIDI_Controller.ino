/*
 * MIDI Controller.ino
 * Created: 10/10/2018
 * Authors: Konstantin Mednikov & Alex Bely
 */ 

#include "MIDIUSB.h"
#include <EEPROM.h>

// EEPROM Memory block
const uint8_t numberOfControls = 128;
uint8_t channels[numberOfControls];
uint8_t controls[numberOfControls];
bool firstRun = true;

// Controls block
const uint8_t analogPins[] = { A0 }; // A1, A2, A3, A4, A5, A6, A7 };
const size_t nb_analogPins = sizeof(analogPins);
const uint8_t addressPins[] = { 2, 3, 4, 5 };
const size_t nb_addressPins = sizeof(addressPins); // 4 address pins
const size_t nb_addresses = 1 << nb_addressPins; // 0001 => 10000 i.e. 16 pots per mux

uint8_t previousValues[nb_analogPins][nb_addressPins] = {}; // previous values

void setup() {
  MemoryRead();
  //firstRun = true; // uncomment for debugging EEPROM
  
  if (firstRun){
    for (int i = 0; i < numberOfControls; i++){
      channels[i] = 1;}
    
    for (int i = 0; i < numberOfControls; i++){
      controls[i] = i+1; } }
  
  Serial.begin(115200);

  for(int i = 0; i < nb_addressPins; i++) {
    pinMode(addressPins[i], OUTPUT); }
}

void setMuxAddress(uint8_t address) {
  for (unsigned int i = 0; i < nb_addressPins; i++) {
    digitalWrite(addressPins[i], address & (1 << i)); 
    }
}

int analogReadMux(uint8_t analogPin, uint8_t address) {
  setMuxAddress(address); // select the right input of the multiplexer
  analogRead(analogPin);  // Throw away first reading

  // avaraging out 10 readings
  // could be decreased with the capacitor
  int sum = 0;
  for(int i = 0; i < 10; i++){
    sum = sum + analogRead(analogPin);
  } 
  sum = sum/10;      
  
  return sum;
}

// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, float value) {
   midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
   MidiUSB.sendMIDI(event);
   MidiUSB.flush();
}

void loop() {
     for (unsigned int pinIndex = 0; pinIndex < nb_analogPins; pinIndex++) {
      for (uint8_t address = 0; address < 2; address++) {
         int newAnalogValue = analogReadMux(analogPins[pinIndex], address);
         uint8_t newMidiValue = newAnalogValue >> 3; // map from 10 bit to 7 bit
         if (newMidiValue != previousValues[pinIndex][address]) { // if the value changed since last time
            controlChange(1, address, newMidiValue); // channel, cc, value
            previousValues[pinIndex][address] = newMidiValue; // the new value will be the previous value the next time the loop repeats
         }
      }
   } 
}
