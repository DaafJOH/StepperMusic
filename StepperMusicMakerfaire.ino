#include "C:\Users\daafs\OneDrive\Documenten\PythonProjects\MIDI\Poke.h"
#include "C:\Users\daafs\OneDrive\Documenten\PythonProjects\MIDI\Pig.h"
#include "C:\Users\daafs\OneDrive\Documenten\PythonProjects\MIDI\Enemy.h"
#include "C:\Users\daafs\OneDrive\Documenten\PythonProjects\MIDI\Hotel.h"

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

int songIndex = 0;

uint32_t mic;

extern const PROGMEM uint16_t Poke_aa[]; // note frequency
extern const PROGMEM uint32_t Poke_ab[]; // note duration
extern const PROGMEM uint32_t Poke_ac[]; // note "prewait", 0 means play note immediately, any other value; wait that many microseconds
extern const int16_t Poke_aLen;
extern const PROGMEM uint16_t Poke_ba[];
extern const PROGMEM uint32_t Poke_bb[];
extern const PROGMEM uint32_t Poke_bc[];
extern const int16_t Poke_bLen;
extern const PROGMEM uint16_t Poke_ca[];
extern const PROGMEM uint32_t Poke_cb[];
extern const PROGMEM uint32_t Poke_cc[];
extern const int16_t Poke_cLen;

extern const PROGMEM uint16_t Pig_aa[]; // note frequency
extern const PROGMEM uint32_t Pig_ab[]; // note duration
extern const PROGMEM uint32_t Pig_ac[]; // note "prewait", 0 means play note immediately, any other value; wait that many microseconds
extern const int16_t Pig_aLen;
extern const PROGMEM uint16_t Pig_ba[];
extern const PROGMEM uint32_t Pig_bb[];
extern const PROGMEM uint32_t Pig_bc[];
extern const int16_t Pig_bLen;
extern const PROGMEM uint16_t Pig_ca[];
extern const PROGMEM uint32_t Pig_cb[];
extern const PROGMEM uint32_t Pig_cc[];
extern const int16_t Pig_cLen;

extern const PROGMEM uint16_t Enemy_aa[]; // note frequency
extern const PROGMEM uint32_t Enemy_ab[]; // note duration
extern const PROGMEM uint32_t Enemy_ac[]; // note "prewait", 0 means play note immediately, any other value; wait that many microseconds
extern const int16_t Enemy_aLen;
extern const PROGMEM uint16_t Enemy_ba[];
extern const PROGMEM uint32_t Enemy_bb[];
extern const PROGMEM uint32_t Enemy_bc[];
extern const int16_t Enemy_bLen;
extern const PROGMEM uint16_t Enemy_ca[];
extern const PROGMEM uint32_t Enemy_cb[];
extern const PROGMEM uint32_t Enemy_cc[];
extern const int16_t Enemy_cLen;

extern const PROGMEM uint16_t Hotel_aa[]; // note frequency
extern const PROGMEM uint32_t Hotel_ab[]; // note duration
extern const PROGMEM uint32_t Hotel_ac[]; // note "prewait", 0 means play note immediately, any other value; wait that many microseconds
extern const int16_t Hotel_aLen;
extern const PROGMEM uint16_t Hotel_ba[];
extern const PROGMEM uint32_t Hotel_bb[];
extern const PROGMEM uint32_t Hotel_bc[];
extern const int16_t Hotel_bLen;
extern const PROGMEM uint16_t Hotel_ca[];
extern const PROGMEM uint32_t Hotel_cb[];
extern const PROGMEM uint32_t Hotel_cc[];
extern const int16_t Hotel_cLen;

uint16_t aa; // note frequency
uint32_t ab; // note duration
uint32_t ac; // note "prewait", 0 means play note immediately, any other value; wait that many microseconds
int16_t aLen;
uint16_t ba;
uint32_t bb;
uint32_t bc;
int16_t bLen;
uint16_t ca;
uint32_t cb;
uint32_t cc;
int16_t cLen;

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
pinMode(10, INPUT_PULLUP); //Button pin
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
digitalWrite(6, LOW);
digitalWrite(7, LOW);
digitalWrite(8, HIGH);
digitalWrite(9, LOW);
}

void loop() {
  if (!digitalRead(10)) {
    mic = micros();
    a0 = mic+100000;
    b0 = mic+100000;
    c0 = mic+100000;
    a1=0;b1=0;c1=0;
    aIndex = -1; bIndex = -1; cIndex = -1;
    digitalWrite(8, LOW);
    if (songIndex == 0) {
      while(aIndex < Poke_aLen && bIndex < Poke_bLen && cIndex < Poke_cLen) {
        mic = micros();
        if (aIndex < Poke_aLen){
          if (mic > a0) { // if note a is finished
            aIndex += 1;
            a0 += pgm_read_dword(&Poke_ab[aIndex]) + pgm_read_dword(&Poke_ac[aIndex]);
            a1 = mic + pgm_read_dword(&Poke_ac[aIndex]);
            aDir = !aDir;
            digitalWrite(2, aDir);}
          else if (mic > a1) {
            digitalWrite(3, HIGH);
            digitalWrite(3, LOW);
            a1 = mic + pgm_read_word(&Poke_aa[aIndex]);}}
        if (bIndex < Poke_bLen){
          if (mic > b0) { // if note a is finished
            bIndex += 1;
            b0 += pgm_read_dword(&Poke_bb[bIndex]) + pgm_read_dword(&Poke_bc[bIndex]);
            b1 = mic + pgm_read_dword(&Poke_bc[bIndex]);
            bDir = !bDir;
            digitalWrite(4, bDir);}
          else if (mic > b1) {
            digitalWrite(5, HIGH);
            digitalWrite(5, LOW);
            b1 = mic + pgm_read_word(&Poke_ba[bIndex]);}}
        if (cIndex < Poke_cLen){
          if (mic > c0) { // if note a is finished
            cIndex += 1;
            c0 += pgm_read_dword(&Poke_cb[cIndex]) + pgm_read_dword(&Poke_cc[cIndex]);
            c1 = mic + pgm_read_dword(&Poke_cc[cIndex]);
            cDir = !cDir;
            digitalWrite(6, cDir);}
          else if (mic > c1) {
            digitalWrite(7, HIGH);
            digitalWrite(7, LOW);
            c1 = mic + pgm_read_word(&Poke_ca[cIndex]);}}}}
    else if (songIndex == 1) {
      while(aIndex < Pig_aLen && bIndex < Pig_bLen && cIndex < Pig_cLen) {
        mic = micros();
        if (aIndex < Pig_aLen){
          if (mic > a0) { // if note a is finished
            aIndex += 1;
            a0 += pgm_read_dword(&Pig_ab[aIndex]) + pgm_read_dword(&Pig_ac[aIndex]);
            a1 = mic + pgm_read_dword(&Pig_ac[aIndex]);
            aDir = !aDir;
            digitalWrite(2, aDir);}
          else if (mic > a1) {
            digitalWrite(3, HIGH);
            digitalWrite(3, LOW);
            a1 = mic + pgm_read_word(&Pig_aa[aIndex]);}}
        if (bIndex < Pig_bLen){
          if (mic > b0) { // if note a is finished
            bIndex += 1;
            b0 += pgm_read_dword(&Pig_bb[bIndex]) + pgm_read_dword(&Pig_bc[bIndex]);
            b1 = mic + pgm_read_dword(&Pig_bc[bIndex]);
            bDir = !bDir;
            digitalWrite(4, bDir);}
          else if (mic > b1) {
            digitalWrite(5, HIGH);
            digitalWrite(5, LOW);
            b1 = mic + pgm_read_word(&Pig_ba[bIndex]);}}
        if (cIndex < Pig_cLen){
          if (mic > c0) { // if note a is finished
            cIndex += 1;
            c0 += pgm_read_dword(&Pig_cb[cIndex]) + pgm_read_dword(&Pig_cc[cIndex]);
            c1 = mic + pgm_read_dword(&Pig_cc[cIndex]);
            cDir = !cDir;
            digitalWrite(6, cDir);}
          else if (mic > c1) {
            digitalWrite(7, HIGH);
            digitalWrite(7, LOW);
            c1 = mic + pgm_read_word(&Pig_ca[cIndex]);}}}}
    else if (songIndex == 2) {
      while(aIndex < Enemy_aLen && bIndex < Enemy_bLen && cIndex < Enemy_cLen) {
        mic = micros();
        if (aIndex < Enemy_aLen){
          if (mic > a0) { // if note a is finished
            aIndex += 1;
            a0 += pgm_read_dword(&Enemy_ab[aIndex]) + pgm_read_dword(&Enemy_ac[aIndex]);
            a1 = mic + pgm_read_dword(&Enemy_ac[aIndex]);
            aDir = !aDir;
            digitalWrite(2, aDir);}
          else if (mic > a1) {
            digitalWrite(3, HIGH);
            digitalWrite(3, LOW);
            a1 = mic + pgm_read_word(&Enemy_aa[aIndex]);}}
        if (bIndex < Enemy_bLen){
          if (mic > b0) { // if note a is finished
            bIndex += 1;
            b0 += pgm_read_dword(&Enemy_bb[bIndex]) + pgm_read_dword(&Enemy_bc[bIndex]);
            b1 = mic + pgm_read_dword(&Enemy_bc[bIndex]);
            bDir = !bDir;
            digitalWrite(4, bDir);}
          else if (mic > b1) {
            digitalWrite(5, HIGH);
            digitalWrite(5, LOW);
            b1 = mic + pgm_read_word(&Enemy_ba[bIndex]);}}
        if (cIndex < Enemy_cLen){
          if (mic > c0) { // if note a is finished
            cIndex += 1;
            c0 += pgm_read_dword(&Enemy_cb[cIndex]) + pgm_read_dword(&Enemy_cc[cIndex]);
            c1 = mic + pgm_read_dword(&Enemy_cc[cIndex]);
            cDir = !cDir;
            digitalWrite(6, cDir);}
          else if (mic > c1) {
            digitalWrite(7, HIGH);
            digitalWrite(7, LOW);
            c1 = mic + pgm_read_word(&Enemy_ca[cIndex]);}}}}
    else if (songIndex == 3) {
      while(aIndex < Hotel_aLen && bIndex < Hotel_bLen && cIndex < Hotel_cLen) {
        mic = micros();
        if (aIndex < Hotel_aLen){
          if (mic > a0) { // if note a is finished
            aIndex += 1;
            a0 += pgm_read_dword(&Hotel_ab[aIndex]) + pgm_read_dword(&Hotel_ac[aIndex]);
            a1 = mic + pgm_read_dword(&Hotel_ac[aIndex]);
            aDir = !aDir;
            digitalWrite(2, aDir);}
          else if (mic > a1) {
            digitalWrite(3, HIGH);
            digitalWrite(3, LOW);
            a1 = mic + pgm_read_word(&Hotel_aa[aIndex]);}}
        if (bIndex < Hotel_bLen){
          if (mic > b0) { // if note a is finished
            bIndex += 1;
            b0 += pgm_read_dword(&Hotel_bb[bIndex]) + pgm_read_dword(&Hotel_bc[bIndex]);
            b1 = mic + pgm_read_dword(&Hotel_bc[bIndex]);
            bDir = !bDir;
            digitalWrite(4, bDir);}
          else if (mic > b1) {
            digitalWrite(5, HIGH);
            digitalWrite(5, LOW);
            b1 = mic + pgm_read_word(&Hotel_ba[bIndex]);}}
        if (cIndex < Hotel_cLen){
          if (mic > c0) { // if note a is finished
            cIndex += 1;
            c0 += pgm_read_dword(&Hotel_cb[cIndex]) + pgm_read_dword(&Hotel_cc[cIndex]);
            c1 = mic + pgm_read_dword(&Hotel_cc[cIndex]);
            cDir = !cDir;
            digitalWrite(6, cDir);}
          else if (mic > c1) {
            digitalWrite(7, HIGH);
            digitalWrite(7, LOW);
            c1 = mic + pgm_read_word(&Hotel_ca[cIndex]);}}}}
    songIndex += 1;
    if (songIndex > 3) {songIndex = 0;}
    digitalWrite(8, HIGH);
  }
}
