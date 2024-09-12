#include "C:\Users\daafs\OneDrive\Documenten\PythonProjects\MIDI\song.h"

uint32_t a0 = 0; // finish flag; if more than micros, note is finished
uint32_t a1 = 0; // temp flag; used for pulsing a frequency or waiting
uint32_t b0 = 0;
uint32_t b1 = 0;
uint32_t c0 = 0;
uint32_t c1 = 0;

int16_t aIndex = -1; // index for which note is playing
int16_t bIndex = -1;
int16_t cIndex = -1;

bool aDir = 0;
bool bDir = 0;
bool cDir = 0;

uint32_t mic;

extern const PROGMEM uint16_t aa[]; // note frequency
extern const PROGMEM uint32_t ab[]; // note duration
extern const PROGMEM uint32_t ac[]; // note "prewait", 0 means play note immediately, any other value; wait that many microseconds
extern const int16_t aLen;
extern const PROGMEM uint16_t ba[];
extern const PROGMEM uint32_t bb[];
extern const PROGMEM uint32_t bc[];
extern const int16_t bLen;
extern const PROGMEM uint16_t ca[];
extern const PROGMEM uint32_t cb[];
extern const PROGMEM uint32_t cc[];
extern const int16_t cLen;

void setup() {
  // put your setup code here, to run once:
pinMode(2, OUTPUT); // Dir Motor 1
pinMode(3, OUTPUT); // Step Motor 1
pinMode(4, OUTPUT); // Dir Motor 2
pinMode(5, OUTPUT); // Step Motor 2
pinMode(6, OUTPUT); // Dir Motor 3
pinMode(7, OUTPUT); // Step Motor 3
pinMode(8, OUTPUT); // Enable pin
pinMode(9, OUTPUT); // Half Step pin
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
digitalWrite(6, LOW);
digitalWrite(7, LOW);
digitalWrite(8, LOW);
digitalWrite(9, HIGH);
mic = micros();
a0 = mic+100000;
b0 = mic+100000;
c0 = mic+100000;
}

void loop() {
  // a
  mic = micros();
  if (aIndex < aLen){
    if (mic > a0) { // if note a is finished
      aIndex += 1;
      a0 += pgm_read_dword(&ab[aIndex]) + pgm_read_dword(&ac[aIndex]);
      a1 = mic + pgm_read_dword(&ac[aIndex]);
      aDir = !aDir;
      digitalWrite(2, aDir);}
    else if (mic > a1) {
      digitalWrite(3, HIGH);
      digitalWrite(3, LOW);
      a1 = mic + pgm_read_word(&aa[aIndex]);}}
  if (bIndex < bLen){
    if (mic > b0) { // if note a is finished
      bIndex += 1;
      b0 += pgm_read_dword(&bb[bIndex]) + pgm_read_dword(&bc[bIndex]);
      b1 = mic + pgm_read_dword(&bc[bIndex]);
      bDir = !bDir;
      digitalWrite(4, bDir);}
    else if (mic > b1) {
      digitalWrite(5, HIGH);
      digitalWrite(5, LOW);
      b1 = mic + pgm_read_word(&ba[bIndex]);}}
  if (cIndex < cLen){
    if (mic > c0) { // if note a is finished
      cIndex += 1;
      c0 += pgm_read_dword(&cb[cIndex]) + pgm_read_dword(&cc[cIndex]);
      c1 = mic + pgm_read_dword(&cc[cIndex]);
      cDir = !cDir;
      digitalWrite(6, cDir);}
    else if (mic > c1) {
      digitalWrite(7, HIGH);
      digitalWrite(7, LOW);
      c1 = mic + pgm_read_word(&ca[cIndex]);}}
  if (aIndex >= aLen && bIndex >= bLen && cIndex >= cLen) {delay(2000); aIndex = -1; bIndex = -1; cIndex = -1; a0=0;a1=0;b0=0;b1=0;c0=0;c1=0;}
}
