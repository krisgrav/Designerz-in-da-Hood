#include "FastLED.h"  //Pakke for kontroll av LED-strips


/*  
 *  Oppset av LED-strips
 */
const int LEDpin = 11;
const int NUM_LEDS = 27;
 
CRGB LEDs[NUM_LEDS];  //Liste som inneholder hver enkelt LED på LED-strip

CHSV rod = CHSV(0, 255, 255);   //Diverse CHSV farger
  CHSV svakRod = CHSV(0, 230, 100);
  
CHSV gronn = CHSV(96, 255, 255);
  CHSV svakGronn = CHSV(96, 230, 100);
  
CHSV blaa = CHSV(160, 255, 255);
  CHSV svakBlaa = CHSV(160, 230, 100);
  
CHSV oransje = CHSV(32, 255, 255);
  CHSV svakOransje = CHSV(32, 230, 100);
  
CHSV gul = CHSV(64, 255,  255);
  CHSV svakGul = CHSV(64, 230,  100);
  
CHSV lilla = CHSV(192, 255, 255);
  CHSV svakLilla = CHSV(192, 230, 100);
  
CHSV rosa = CHSV(224, 255, 255);
  CHSV svakRosa = CHSV(224, 230, 100);

CHSV av = CHSV(0, 0, 0);

CHSV sterkeFarger[] = {rod, gronn, oransje, gul, lilla, rosa};
CHSV svakeFarger[] = {svakRod, svakGronn, svakOransje, svakGul, svakLilla, svakRosa};
CHSV sterkeOgSvakeFarger[2]; //skal inneholde en sterk og en svak farge. [0] = sterkfarge, [1] = svakfarge.

//Lister over LEDs i rekkefølger på bingobrett
int ledVenstreKol[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
int ledMidtKol[] = {17, 16, 15, 14, 13, 12, 11, 10, 9};
int ledHoyreKol[] = {18, 19, 20, 21, 22, 23, 24, 25, 26};
int ledOversteRad[] = {0, 1, 2, 15, 16, 17, 18, 19, 20};
int ledMidtRad[] = {3, 4, 5, 12, 13, 14, 21, 22, 23};
int ledBunnRad[] = {6, 7, 8, 9, 10, 11, 24, 25, 26};
int ledDia1[] = {0, 1, 2, 14, 13, 12, 24, 25, 26};
int ledDia2[] = {18, 19, 20, 14, 13, 12, 6, 7, 8};


/*
 * Oppsett av knapper
 */
const int knapp11 = 2;
const int knapp12 = 3;
const int knapp13 = 4;
const int knapp21 = 5;
const int knapp22 = 6;
const int knapp23 = 7;
const int knapp31 = 8;
const int knapp32 = 9;
const int knapp33 = 10;


/*
 * Variabler til bruk i bingo-spill
 */
boolean litenbingo = false;
boolean storbingo = false;
boolean harfaattlitenbingo = false; //brukes ikke?
int storBingoTeller = 0;

boolean trykket11 = false;
boolean trykket12 = false;
boolean trykket13 = false;
boolean trykket21 = false;
boolean trykket22 = false;
boolean trykket23 = false;
boolean trykket31 = false;
boolean trykket32 = false;
boolean trykket33 = false;


/*
 * Oppsett av Piezo
 */
int piezo = 12; //Piezo-pin

boolean lydPaa = true;

const int toneC4 = 262;   //Diverse toner satt opp etter frekvens
const int toneD4 = 294;
const int toneE4 = 330;
const int toneF4 = 349;
const int toneG4 = 392;
  const int toneGiss = 415;
const int toneA4 = 440;
const int toneB4 = 494;
const int toneC5 = 523;
const int toneD5 = 587;

int toneSkala[] = {toneC4, toneD4, toneE4, toneF4, toneG4, toneA4, toneB4, toneC5};




void setup() {
//  pinMode(resetknapp, INPUT);
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LEDpin>(LEDs, NUM_LEDS);
 // pinMode(testKnapp, INPUT);
  
  pinMode(knapp11, INPUT);
  pinMode(knapp12, INPUT);
  pinMode(knapp13, INPUT);
  pinMode(knapp21, INPUT);
  pinMode(knapp22, INPUT);
  pinMode(knapp23, INPUT);
  pinMode(knapp31, INPUT);
  pinMode(knapp32, INPUT);
  pinMode(knapp33, INPUT);
  digitalWrite(knapp11, LOW);
  digitalWrite(knapp12, LOW);
  digitalWrite(knapp13, LOW);
  digitalWrite(knapp21, LOW);
  digitalWrite(knapp22, LOW);
  digitalWrite(knapp23, LOW);
  digitalWrite(knapp31, LOW);
  digitalWrite(knapp32, LOW);
  digitalWrite(knapp33, LOW);
  
  pinMode(12, OUTPUT);
  alleAv();

}


/*
 * Loopen sjekker kontinuerlig om hver enkelt knapp er trykket ned, før den deretter sjekker om kombinasjonen av knapper
 * tilsvarer en stor eller liten bingo.
 * Om ingen knapper er trykket ned vil brettet resette variablene som omhandler stor eller liten bingo.
 */

void loop() {
  angre();
  k11();
  k12();
  k13();
  k21();
  k22();
  k23();
  k31();
  k32();
  k33();
  if(!(litenbingo)){ //Hvis det ikke er en liten bingo ser brettet etter en liten bingo.
    bingoRad();
    bingoKol();
    bingoDia();
  }
  else if(litenbingo){  //Når det er liten bingo ser brettet etter en stor bingo.
    bingoStor();
  }  
}

/*
 * Metode for tilbakestilling av brettet.
 * Om ingen av knappene er trykket ned (aktive) vil brettet kalle på reset() metoden.
 */
void angre(){
  if((!(trykket11)) && (!(trykket12)) && (!(trykket13)) && (!(trykket21)) && (!(trykket22)) && (!(trykket23))
  && (!(trykket31)) && (!(trykket32)) && (!(trykket33))){
    reset();
  }
}


/*
 * De følgende metodene sjekker hver enkkelt knapp om de er trykket inn.
 * Deretter kalles metode for om knappen er av eller på, og feedback i form av en tone spilles.
 * Knappene metodene har navn etter kanppenes rad og kolonne. (k11 = knapp på rad 1, kolonne 1. k23 = knapp på rad 2, kolonne 3)
 */
void k11() {
  if (digitalRead(knapp11) == HIGH) {
    delay(50);
    if(!(trykket11)){
      knappeTrykkPaa(11);
      tone(piezo, toneC4); 
      delay(100);
      noTone(12);
      delay(100);
    }
    else if(trykket11){
      knappeTrykkAv(11);
      tone(piezo, toneC4);
      delay(100);
      noTone(12);
      delay(100);
    }
  }
}

void k12() {
  if (digitalRead(knapp12) == HIGH) {
    delay(50);
    if(!(trykket12)){
      knappeTrykkPaa(12);
      tone(piezo, toneD4);
      delay(100);
      noTone(12);
      delay(100);
    }
    else if(trykket12){
      knappeTrykkAv(12);
      tone(piezo, toneD4);
      delay(100);
      noTone(12);
      delay(100);
    }    
  }
}

void k13() {
  if (digitalRead(knapp13) == HIGH) {
    delay(50);
    if(!(trykket13)){
      knappeTrykkPaa(13);
      tone(piezo, toneE4);
      delay(100);
      noTone(12);
      delay(100);
    }
    else if(trykket13){
      knappeTrykkAv(13);
      tone(piezo, toneE4);
      delay(100);
      noTone(12);
      delay(100);
    }
  }
}

void k21() {
  if (digitalRead(knapp21) == HIGH) {
    delay(50);
    if(!(trykket21)){
      knappeTrykkPaa(21);
      tone(piezo, toneF4);
      delay(100);
      noTone(piezo);
      delay(100);
    }
    else if(trykket21){
      knappeTrykkAv(21);
      tone(piezo, toneF4);
      delay(100);
      noTone(piezo);
      delay(100);
    }
  }
}

void k22() {
  if (digitalRead(knapp22) == HIGH) {
    delay(50);
    if(!(trykket22)){
      knappeTrykkPaa(22);
      tone(piezo, toneG4);
      delay(100);
      noTone(piezo);
      delay(100);
    }
    else if(trykket22){
      knappeTrykkAv(22);
      tone(piezo, toneG4);
      delay(100);
      noTone(piezo);
      delay(100);
    }
  }
}

void k23() {
  if (digitalRead(knapp23) == HIGH) {
    delay(50);
    if(!(trykket23)){
      knappeTrykkPaa(23);
      tone(piezo, toneA4);
      delay(100);
      noTone(piezo);
      delay(100);
    }
    else if(trykket23){
      knappeTrykkAv(23);
      tone(piezo, toneA4);
      delay(100);
      noTone(piezo);
      delay(100);
    }  
  }
}

void k31() {
  if (digitalRead(knapp31) == HIGH) {
    delay(50);
    if(!(trykket31)){
      knappeTrykkPaa(31);
      tone(piezo, toneB4);
      delay(100);
      noTone(piezo);
      delay(100);
    }
    else if(trykket31){
      knappeTrykkAv(31);
      tone(piezo, toneB4);
      delay(100);
      noTone(piezo);
      delay(100);
    }
  }
}

void k32() {
  if (digitalRead(knapp32) == HIGH) {
    delay(50);
    if(!(trykket32)){
      knappeTrykkPaa(32);
      tone(piezo, toneC5);
      delay(100);
      noTone(piezo);
      delay(100);
    }
    else if(trykket32){
      knappeTrykkAv(32);
      tone(piezo, toneC5);
      delay(100);
      noTone(piezo);
      delay(100);
    }
  }
}

void k33() {
  if (digitalRead(knapp33) == HIGH) {
    delay(50);
    if(!(trykket33)){
      knappeTrykkPaa(33);
      tone(piezo, toneD5);
      delay(100);
      noTone(piezo);
      delay(100);
    }
    else if(trykket33){
      knappeTrykkAv(33);
      tone(piezo, toneD5);
      delay(100);
      noTone(piezo);
      delay(100);
    }
  }
}


/*
 * Metoden sjekker om det finnes en horisontal liten bingo
 */
void bingoRad(){
  if(trykket11 && trykket12 && trykket13){
    litenbingo = true;  //Liten bingo settes til true
    litenBingoTone();   //Spiller feedback til brukeren
    Serial.println("Rad 1 BINGO");    //Printer til terminal (kun brukt under utvikling)
    litenBingoFlash(ledOversteRad);   //Viser feedback i form av lys på bingoraden
    litenBingoFlash(ledOversteRad);
    litenBingoLys(11, 13);    //Setter faste lys for å indikere bingo på den gjeldene raden
    return;
  }
  if(trykket21 && trykket22 && trykket23){
    litenbingo = true;
    litenBingoTone();
    Serial.println("Rad 2 BINGO");
    litenBingoFlash(ledMidtRad);
    litenBingoFlash(ledMidtRad);
    litenBingoLys(21, 23);
    return;
  }
  if(trykket31 && trykket32 && trykket33){
    litenbingo = true;
    litenBingoTone();
    Serial.println("Rad 3 BINGO");
    litenBingoFlash(ledBunnRad);
    litenBingoFlash(ledBunnRad);
    litenBingoLys(31, 33);
    return;
  }
}

/*
 * Metoden sjekker om det finnes en vertikal liten bingo.
 * Ellers er den lik bingoRad()
 */
void bingoKol(){
  if(trykket11 && trykket21 && trykket31){
    litenbingo = true;
    litenBingoTone();
    Serial.println("Kol 1 BINGO");
    litenBingoFlash(ledVenstreKol);
    litenBingoFlash(ledVenstreKol);
    litenBingoLys(11, 31);
    return;
  }
  
  if(trykket12 && trykket22 && trykket32){
    litenbingo = true;
    litenBingoTone();
    Serial.println("Kol 2 BINGO");
    litenBingoFlash(ledMidtKol);
    litenBingoFlash(ledMidtKol);
    litenBingoLys(12, 32);
    return;
  }
  
  if(trykket13 && trykket23 && trykket33){
    litenbingo = true;
    litenBingoTone();
    Serial.println("Kol 3 BINGO");
    litenBingoFlash(ledHoyreKol);
    litenBingoFlash(ledHoyreKol);
    litenBingoLys(13, 33);
    return;
  }
}


/*
 * Metoden sjekker om det finnes en diagonal liten bingo.
 * Ellers er den lik bingoRad() og bingoKol().
 */
void bingoDia(){
  if(trykket11 && trykket22 && trykket33){
    litenbingo = true;
    litenBingoTone();
    Serial.println("Dia 1 BINGO");
    litenBingoFlash(ledDia1);
    litenBingoFlash(ledDia1);
    litenBingoLys(11, 33);
    return;
  }
  
  if(trykket31 && trykket22 && trykket13){
    litenbingo = true;
    litenBingoTone();
    Serial.println("Dia 2 BINGO");
    litenBingoFlash(ledDia2);
    litenBingoFlash(ledDia2);
    litenBingoLys(31, 13);
    return;
  }
}

/*
 * Metoden sjekker om det finnes en stor bingo.
 */
void bingoStor(){
  if(litenbingo){   //Hvis bruker har fått en liten bingo
    if(trykket11 && trykket12 && trykket13 && trykket21 && trykket22 && trykket23 && trykket31 && trykket32 && trykket33){  //Hvis alle knapper på brettet er trykket (aktive).
      if(storBingoTeller < 3){    //Hvis bruker har fått færren enn 2 store bingo.
        storbingo = true;
        storBingoTone();    //Spiller feedback for brukeren.
        Serial.println("Stor Bingo!");    //Printer til terminal (kun brukt under utvikling).
        storBingoFlash();   //Viser feedback i form av lys på bingobrettet.
        blinkAlle();    //Viser feedback i form av lys på bingobrettet.
        reset();    //Tilbakestiller alle variabler og LEDs
        storBingoTeller++;    //Teller antall ganger bruker har fått stor bingo.
        delay(1000);
      }
      else if(storBingoTeller = 3){ //Hvis bruker har fått 3x stro bingo.
        storbingo = true;
        inParis();    //Spiller unik tone
        inParis();    //Spiller unik tone
        Serial.println("Stor Bingo!");
        storBingoFlash();
        blinkAlle();
        reset();
        storBingoTeller = 0;    //Tilbakestiller telleren.
        delay(1000);
      }
    }
  }
}


/*
 * Oppsett av lyssignal.
 * Sender lyslenke av fire LEDs en runde over brettet, først rød -> grønn -> blå.
 */
void lys1(int hastighet){
  for(int i = 0; i < NUM_LEDS; i++){ //rød
    LEDs[i] = rod;
    LEDs[i + 1] = rod;
    LEDs[i + 2] = rod;
    LEDs[i + 3] = rod;
    FastLED.show();
    delay(hastighet);
    LEDs[i] = av;
    LEDs[i + 1] = av;
    LEDs[i + 2] = av;
    LEDs[i + 3] = av;
    FastLED.show();
  }
  for(int i = 0; i < NUM_LEDS; i++){ //grønn
    LEDs[i] = gronn;
    LEDs[i + 1] = gronn;
    LEDs[i + 2] = gronn;
    LEDs[i + 3] = gronn;
    FastLED.show();
    delay(hastighet);
    LEDs[i] = av;
    LEDs[i + 1] = av;
    LEDs[i + 2] = av;
    LEDs[i + 3] = av;
    FastLED.show();
  }
  for(int i = 0; i < NUM_LEDS; i++){ //blå
    LEDs[i] = blaa;
    LEDs[i + 1] = blaa;
    LEDs[i + 2] = blaa;
    LEDs[i + 3] = blaa;
    FastLED.show();
    delay(hastighet);
    LEDs[i] = av;
    LEDs[i + 1] = av;
    LEDs[i + 2] = av;
    LEDs[i + 3] = av;
    FastLED.show();
  }
}


/*
 * Oppsett av lyssignal.
 * Blinker hvert syvende lys, hopper så en LED frem og blinker hvert syvende lys.
 */
void blink1(CHSV farge, int startLED, int hastighet){
  for(int i = startLED; i < NUM_LEDS; i += 7){
    LEDs[i] = farge; 
  }
  FastLED.show();
  delay(hastighet);
  alleAv();
  blink1(rod, startLED +1, hastighet);
}


/*
 * Skrur av alle LEDs
 */
void alleAv(){
  for(int i = 0; i < NUM_LEDS; i++){
    LEDs[i] = av;
  }
  FastLED.show();
}


/*
 * Metode som kalles når en knapp blir trykket inn (aktiv).
 * Tar knappen som parameter.
 */
void knappeTrykkPaa(int knapp){
  if(knapp == 11){
    settFarger();   //Kaller matode som finnner en sterk og en svak farge (feks sterk rød og svak rød).
    LEDs[0] = sterkeOgSvakeFarger[1]; //Setter svake farger i ytterkanten av knappen.
    LEDs[1] = sterkeOgSvakeFarger[0]; //Setter sterk farge i senter av knappen.
    LEDs[2] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket11 = true;
    
  }
  if(knapp == 21){
    settFarger();
    LEDs[3] = sterkeOgSvakeFarger[1];
    LEDs[4] = sterkeOgSvakeFarger[0];
    LEDs[5] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket21 = true;
  }
  if(knapp == 31){
    settFarger();
    LEDs[6] = sterkeOgSvakeFarger[1];
    LEDs[7] = sterkeOgSvakeFarger[0];
    LEDs[8] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket31 = true;
  }
  if(knapp == 32){
    settFarger();
    LEDs[11] = sterkeOgSvakeFarger[1];
    LEDs[10] = sterkeOgSvakeFarger[0];
    LEDs[9] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket32 = true;
  }
  if(knapp == 22){
    settFarger();
    LEDs[14] = sterkeOgSvakeFarger[1];
    LEDs[13] = sterkeOgSvakeFarger[0];
    LEDs[12] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket22 = true;
  }
  if(knapp == 12){
    settFarger();
    LEDs[15] = sterkeOgSvakeFarger[1];
    LEDs[16] = sterkeOgSvakeFarger[0];
    LEDs[17] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket12 = true;
  }
  if(knapp == 13){
    settFarger();
    LEDs[18] = sterkeOgSvakeFarger[1];
    LEDs[19] = sterkeOgSvakeFarger[0];
    LEDs[20] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket13 = true;
  }
  if(knapp == 23){
    settFarger();
    LEDs[21] = sterkeOgSvakeFarger[1];
    LEDs[22] = sterkeOgSvakeFarger[0];
    LEDs[23] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket23 = true;
  }
  if(knapp == 33){
    settFarger();
    LEDs[24] = sterkeOgSvakeFarger[1];
    LEDs[25] = sterkeOgSvakeFarger[0];
    LEDs[26] = sterkeOgSvakeFarger[1];
    FastLED.show();
    trykket33 = true;
  }
}


/*
 * Metode som kalles når en knapp trykkes av (deaktiveres).
 * Tar knappen som parameter.
 */
void knappeTrykkAv(int knapp){
  if(knapp == 11){
    LEDs[0] = av;   //Skrur av lyset på den gjeldende knappen.
    LEDs[1] = av;
    LEDs[2] = av;
    FastLED.show();
    trykket11 = false;
  }
  if(knapp == 21){
    LEDs[3] = av;
    LEDs[4] = av;
    LEDs[5] = av;
    FastLED.show();
    trykket21 = false;
  }
  if(knapp == 31){
    LEDs[6] = av;
    LEDs[7] = av;
    LEDs[8] = av;
    FastLED.show();
    trykket31 = false;
  }
  if(knapp == 32){
    LEDs[9] = av;
    LEDs[10] = av;
    LEDs[11] = av;
    FastLED.show();
    trykket32 = false;
  }
  if(knapp == 22){
    LEDs[12] = av;
    LEDs[13] = av;
    LEDs[14] = av;
    FastLED.show();
    trykket22 = false;
  }
  if(knapp == 12){
    LEDs[15] = av;
    LEDs[16] = av;
    LEDs[17] = av;
    FastLED.show();
    trykket12 = false;
  }
  if(knapp == 13){
    LEDs[18] = av;
    LEDs[19] = av;
    LEDs[20] = av;
    FastLED.show();
    trykket13 = false;
  }
  if(knapp == 23){
    LEDs[21] = av;
    LEDs[22] = av;
    LEDs[23] = av;
    FastLED.show();
    trykket23 = false;
  }
  if(knapp == 33){
    LEDs[24] = av;
    LEDs[25] = av;
    LEDs[26] = av;
    FastLED.show();
    trykket33 = false;
  }
}

/*
 * Metode som returenre en tilfeldig sterk farge fra listen av sterke farger.
 */
CHSV hentRandomFarge(){
  int farge = random(0, 5); //0-antall farger i fargeliste
  return sterkeFarger[farge];
}

/*
 * Metode som setter en tilfeldig sterk og en svak farge i en array.
 * For eksempel sterk rød på index 0, og svak rød på index 1.
 */
void settFarger(){
  int arrayPlass = random(0, 5);
  sterkeOgSvakeFarger[0] = sterkeFarger[arrayPlass];
  sterkeOgSvakeFarger[1] = svakeFarger[arrayPlass];
}

/*
 * Metode som setter lys for liten bingo.
 */
void litenBingoLys(int knapp1, int knapp2){ //paramter er første og siste knapp i en liten bingorekke
  settFarger();
  if(knapp1 == 11 && knapp2 == 13){ //øverste rad
    LEDs[0] = sterkeOgSvakeFarger[1]; //knapp 11
    LEDs[1] = sterkeOgSvakeFarger[0];
    LEDs[2] = sterkeOgSvakeFarger[1];

    LEDs[15] = sterkeOgSvakeFarger[1]; //knapp 12
    LEDs[16] = sterkeOgSvakeFarger[0];
    LEDs[17] = sterkeOgSvakeFarger[1];

    LEDs[18] = sterkeOgSvakeFarger[1]; //knapp 13
    LEDs[19] = sterkeOgSvakeFarger[0];
    LEDs[20] = sterkeOgSvakeFarger[1];
    FastLED.show();
  }
  else if(knapp1 == 21 && knapp2 == 23){ //midterste rad
    LEDs[3] = sterkeOgSvakeFarger[1]; //knapp 21
    LEDs[4] = sterkeOgSvakeFarger[0];
    LEDs[5] = sterkeOgSvakeFarger[1];

    LEDs[12] = sterkeOgSvakeFarger[1]; //knapp 22
    LEDs[13] = sterkeOgSvakeFarger[0];
    LEDs[14] = sterkeOgSvakeFarger[1];

    LEDs[21] = sterkeOgSvakeFarger[1]; //knapp 23
    LEDs[22] = sterkeOgSvakeFarger[0];
    LEDs[23] = sterkeOgSvakeFarger[1];
    FastLED.show();
  }
  else if(knapp1 == 31 && knapp2 == 33){ //nederste rad
    LEDs[6] = sterkeOgSvakeFarger[1]; //knapp 31
    LEDs[7] = sterkeOgSvakeFarger[0];
    LEDs[8] = sterkeOgSvakeFarger[1];

    LEDs[9] = sterkeOgSvakeFarger[1]; //knapp 32
    LEDs[10] = sterkeOgSvakeFarger[0];
    LEDs[11] = sterkeOgSvakeFarger[1];
    
    LEDs[24] = sterkeOgSvakeFarger[1]; //knapp 33
    LEDs[25] = sterkeOgSvakeFarger[0];
    LEDs[26] = sterkeOgSvakeFarger[1];
    FastLED.show();
  }
  else if(knapp1 == 11 && knapp2 == 31){ //venstre kol
    LEDs[0] = sterkeOgSvakeFarger[0]; //knapp 11
    LEDs[1] = sterkeOgSvakeFarger[0];
    LEDs[2] = sterkeOgSvakeFarger[0];

    LEDs[3] = sterkeOgSvakeFarger[0]; //knapp 21
    LEDs[4] = sterkeOgSvakeFarger[0];
    LEDs[5] = sterkeOgSvakeFarger[0];

    LEDs[6] = sterkeOgSvakeFarger[0]; //knapp 31
    LEDs[7] = sterkeOgSvakeFarger[0];
    LEDs[8] = sterkeOgSvakeFarger[0];
    FastLED.show();
  }
  else if(knapp1 == 12 && knapp2 == 32){ //midtre kol
    LEDs[15] = sterkeOgSvakeFarger[0]; //knapp 12
    LEDs[16] = sterkeOgSvakeFarger[0];
    LEDs[17] = sterkeOgSvakeFarger[0];

    LEDs[12] = sterkeOgSvakeFarger[0];  //knapp 22
    LEDs[13] = sterkeOgSvakeFarger[0];
    LEDs[14] = sterkeOgSvakeFarger[0];

    LEDs[9] = sterkeOgSvakeFarger[0];  //knapp 32
    LEDs[10] = sterkeOgSvakeFarger[0];
    LEDs[11] = sterkeOgSvakeFarger[0];
    FastLED.show();
  }
  else if(knapp1 == 13 && knapp2 == 33){   //høre kol
    LEDs[18] = sterkeOgSvakeFarger[0];  //knapp 13
    LEDs[19] = sterkeOgSvakeFarger[0];
    LEDs[20] = sterkeOgSvakeFarger[0];

    LEDs[21] = sterkeOgSvakeFarger[0];   //knapp 23
    LEDs[22] = sterkeOgSvakeFarger[0];
    LEDs[23] = sterkeOgSvakeFarger[0];

    LEDs[24] = sterkeOgSvakeFarger[0];  //knapp 33
    LEDs[25] = sterkeOgSvakeFarger[0];
    LEDs[26] = sterkeOgSvakeFarger[0];
    FastLED.show();
  }
  else if(knapp1 == 11&& knapp2 == 33){    //dia1
    LEDs[0] = sterkeOgSvakeFarger[1];  //knapp 11
    LEDs[1] = sterkeOgSvakeFarger[0];
    LEDs[2] = sterkeOgSvakeFarger[1];

    LEDs[12] = sterkeOgSvakeFarger[1]; //knapp 22
    LEDs[13] = sterkeOgSvakeFarger[0];
    LEDs[14] = sterkeOgSvakeFarger[1];

    LEDs[24] = sterkeOgSvakeFarger[1];  //knapp33
    LEDs[25] = sterkeOgSvakeFarger[0];
    LEDs[26] = sterkeOgSvakeFarger[1];
    FastLED.show();
  }
  else if(knapp1 == 13 && knapp2 == 31){   //dia2
    LEDs[18] = sterkeOgSvakeFarger[1]; //knapp 13
    LEDs[19] = sterkeOgSvakeFarger[0];
    LEDs[20] = sterkeOgSvakeFarger[1];

    LEDs[12] = sterkeOgSvakeFarger[1]; //knapp 22
    LEDs[13] = sterkeOgSvakeFarger[0];
    LEDs[14] = sterkeOgSvakeFarger[1];

    LEDs[6] = sterkeOgSvakeFarger[1];  //knapp 31
    LEDs[7] = sterkeOgSvakeFarger[0];
    LEDs[8] = sterkeOgSvakeFarger[1];
    FastLED.show();
  }
}

/*
 * Metode som blinker lys til feedback for liten bingo
 */
void litenBingoFlash(int ledListe[]){ //blinker et lys i random farge en rad/kolonne tur/retur
  for(int x = 0; x < 9; x++){
    LEDs[ledListe[x]] = hentRandomFarge();
    //LEDs[ledListe[x + 1]] = hentRandomFarge();
    FastLED.show();
    delay(100);
    LEDs[ledListe[x]] = av;
    LEDs[ledListe[x + 1]] = av;
    FastLED.show();
  }
  for(int x = 8; x >= 0; x--){
    LEDs[ledListe[x]] = hentRandomFarge();
    //LEDs[ledListe[x + 1]] = hentRandomFarge();
    FastLED.show();
    delay(100);
    LEDs[ledListe[x]] = av;
    LEDs[ledListe[x + 1]] = av;
    FastLED.show();
  }  
}

/*
 * Metode som blinker lys til feedback for stor bingo.
 */
void storBingoFlash(){
  alleAv();
  for(int x = 0; x < NUM_LEDS; x++){ //Blinker alle LEDs fra første LED til siste.
    LEDs[x - 1] = hentRandomFarge();
    LEDs[x] = hentRandomFarge();
    LEDs[x + 1] = hentRandomFarge();
    FastLED.show();
    delay(50);
    LEDs[x -1] = av;
    LEDs[x] = av;
    LEDs[x + 1] = av;
    FastLED.show();
  }
  for(int x = 26; x >= 0; x--){ //Blinker alle LEDs fra siste LED til første.
    LEDs[x - 1] = hentRandomFarge();
    LEDs[x] = hentRandomFarge();
    LEDs[x + 1] = hentRandomFarge();
    FastLED.show();
    delay(50);
    LEDs[x -1] = av;
    LEDs[x] = av;
    LEDs[x + 1] = av;
    FastLED.show();
  }
}

/*
 * Metode som blinker alle lys på brettet en gang for alle fargene.
 */
void blinkAlle(){
  for(int x = 0; x < 6; x++){
    allePaa(svakeFarger[x]);
    delay(250);
  }
}

/*
 * Metode som blinker alle lys i en tilfeldig farge et gitt antall ganger.
 */
void blinkAlle2(int antBlink){
  for(int x = 0; x < antBlink; x++){
    CHSV farge = hentRandomFarge();
    allePaa(farge);
    delay(250);
    alleAv();
    delay(250);
  }
}

/*
 * Metode som skrur på alle lys til en bestemt farge.
 */
void allePaa(CHSV farge){
  LEDs[0] = farge; LEDs[1] = farge; LEDs[2] = farge; LEDs[3] = farge;
  LEDs[4] = farge; LEDs[5] = farge; LEDs[6] = farge; LEDs[7] = farge;
  LEDs[8] = farge; LEDs[9] = farge; LEDs[10] = farge; LEDs[11] = farge; 
  LEDs[12] = farge; LEDs[13] = farge; LEDs[14] = farge; LEDs[15] = farge;
  LEDs[16] = farge; LEDs[17] = farge; LEDs[18] = farge; LEDs[19] = farge;
  LEDs[20] = farge; LEDs[21] = farge; LEDs[22] = farge; LEDs[23] = farge;
  LEDs[24] = farge; LEDs[25] = farge; LEDs[26] = farge;
  FastLED.show();
}

/*
 * Reset metode som skrur av alle lys og setter aller spill-variabler til false.
 */
void reset(){
  alleAv();
  
  litenbingo = false;
  storbingo = false;
  harfaattlitenbingo = false;
  
  trykket11 = false;
  trykket12 = false;
  trykket13 = false;
  trykket21 = false;
  trykket22 = false;
  trykket23 = false;
  trykket31 = false;
  trykket32 = false;
  trykket33 = false;
}

/*
 * Metode som spiller en stigende toneskala.
 */
void spillOppgang(){
  for(int x = 0; x < 8; x++){
    tone(piezo, toneSkala[x]);
    delay(125);
  }
  noTone(piezo);
}

/*
 * Metode som spiller en sykende toneskala.
 */
void spillNedgang(){
  for(int x = 8; x >= 0; x--){
    tone(piezo, toneSkala[x]);
    delay(125);
  }
  noTone(piezo); 
}

/*
 * Metode som spiller tone for liten bingo.
 */
void litenBingoTone(){
  spillNedgang();
  spillOppgang();
  spillNedgang();
}

/*
 * Metode som spiller tone for stor bingo.
 */
void storBingoTone(){
  spillOppgang();
  spillOppgang();
  tone(piezo, toneC5);
  delay(500);
  noTone(piezo);
  tone(piezo, toneC5);
  delay(500);
  noTone(piezo);
}

/*
 * Metode som spiler Ni**as In Paris - Kanye West.
 * Brukes til den tredje store bingoen.
 */
void inParis(){
  tone(piezo, toneF4);
  delay(250);
  tone(piezo, toneC4);
  delay(250);
  noTone(piezo);
  delay(500);
  
  tone(piezo, toneGiss);
  delay(250);
  tone(piezo, toneC4);
  delay(250);
  noTone(piezo);
  delay(500);

  tone(piezo, toneG4);
  delay(250);
  tone(piezo, toneC4);
  delay(250);
  noTone(piezo);
  delay(500);

  tone(piezo, toneF4);
  delay(166);
  tone(piezo, toneC4);
  delay(250);
  tone(piezo, toneC5);
  delay(250);
  noTone(piezo);
  delay(500); 
  
}
