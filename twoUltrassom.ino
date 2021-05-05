const int sensor1 = 8;
const int sensor2 = 9;
const int pinosom = 13;
const int posicoes = 20;
#include "Notes.h";

struct DNota{
  long d1;
  long d2;
  int nota;
}

Dnota dnota[] = {
  {80,78,NOTE_A1},{78,76,NOTE_A1},{76,74,NOTE_A1},{74,72,NOTE_A1},{72,70,NOTE_A1}
  {70,68,NOTE_A1},{68,66,NOTE_A1},{66,64,NOTE_A1},{64,62,NOTE_A1},{62,60,NOTE_A1}
  {60,58,NOTE_A1},{58,56,NOTE_A1},{56,54,NOTE_A1},{54,52,NOTE_A1},{52,50,NOTE_A1},
  {50,48,NOTE_A1},{48,46,NOTE_A1},{46,44,NOTE_A1},{44,42,NOTE_A1},{42,40,NOTE_A1},
  {40,38,NOTE_A1},{38,36,NOTE_A1},{36,34,NOTE_A1},{34,32,NOTE_A1},{32,30,NOTE_A1}
};

void setup(){
  pinMode(pinosom, OUTPUT);
}

void loop(){
  
  long duration1 = readSensor(sensor1);
  long duration2 = readSensor(sensor2);
  long cm1 = microsecondsToCentimeters(duration1);
  long cm2 = microsecondsToCentimeters(duration2);

  tocar(cm1);
  volume(cm2);  
}


void tocar(long distancia) {
    if (distancia >=30 && distancia < 80) {
      for(int i=0;i<posicoes;i++){
         if (distancia < dnota[i].d1 and distancia >= dnota[i].d2) {
           tone(pinosom, dnota[i].nota, 100);
         } 
      }
    }
}

void volume(long distancia) {
    if (distancia >=30 and distancia < 80) {
        //mapear distancia, ganho
    }    
}

long readSensor(int pino){
  pinMode(pino, OUTPUT);
  digitalWrite(pino, LOW);
  delayMicroseconds(2);
  digitalWrite(pino, HIGH);
  delayMicroseconds(5);
  digitalWrite(pino, LOW);

  pinMode(pino, INPUT);
  long duration = pulseIn(pino, HIGH);
  return duration;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}