#include <Arduino.h>

// Definování pinu, ke kterému je připojena LED
int led_pin = 3;

// Definování časových intervalů pro Morseovu abecedu
int tecka = 150;    // Délka trvání tečky 
int carka = 300;   // Délka trvání čárky
int pauza = 50;   // Pauza mezi jednotlivými symboly (tečka/čárka) v rámci jednoho písmena
int pispauza = 400;  // Pauza mezi jednotlivými písmeny
int pauzslov = 800;   // Pauza mezi slovy

// 0 = tečka, 1 = čárka.
int morseovka[26][4] = {
    {0, 1},        // A: .-
    {1, 0, 0, 0},  // B: -...
    {1, 0, 1, 0},  // C: -.-.
    {1, 0, 0},     // D: -..
    {0},           // E: .
    {0, 0, 1, 0},  // F: ..-.
    {1, 1, 0},     // G: --.
    {0, 0, 0, 0},  // H: ....
    {0, 0},        // I: ..
    {0, 1, 1, 1},  // J: .---
    {1, 0, 1},     // K: -.-
    {0, 1, 0, 0},  // L: .-..
    {1, 1},        // M: --
    {1, 0},        // N: -.
    {1, 1, 1},     // O: ---
    {0, 1, 1, 0},  // P: .--.
    {1, 1, 0, 1},  // Q: --.-
    {0, 1, 0},     // R: .-.
    {0, 0, 0},     // S: ...
    {1},           // T: -
    {0, 0, 1},     // U: ..-
    {0, 0, 0, 1},  // V: ...-
    {0, 1, 1},     // W: .--
    {1, 0, 0, 1},  // X: -..-
    {1, 0, 1, 1},  // Y: -.--
    {1, 1, 0, 0}   // Z: --..
};

// Pole obsahující počet symbolů (teček/čárek) pro každé písmeno
// Tento údaj umožňuje správně iterovat přes jednotlivé symboly v poli morseovka
int pocet[26] = {2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4, 2, 2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4, 4, 4};

void setup() {
    pinMode(led_pin, OUTPUT); 
    Serial.begin(9600);        
    Serial.println("Zadejt pismenko a ja ti ho prevedu do Morseovy abecedy :33 :");  // Výzva na sériový monitor
}

// Funkce, která převádí daný Morseův symbol na blikání LED
// Parametry:
// - morse_symbol[]: Pole obsahující Morseův kód pro jedno písmeno (tečky/čárky)
// - length: Počet symbolů v Morseově kódu daného písmena
void ledka(int morse_symbol[], int length) {
    for (int i = 0; i < length; i++) {  // Iterace přes jednotlivé symboly (tečky/čárky) Morseova kódu
        if (morse_symbol[i] == 0) {
            // Tečka: krátký blik LED
            digitalWrite(led_pin, HIGH);  // Rozsvícení LED
            delay(tecka);            // Zpoždění podle délky tečky
            digitalWrite(led_pin, LOW);   // Zhasnutí LED
            Serial.print(" .");           // Výpis tečky na sériový monitor
        } else if (morse_symbol[i] == 1) {
            // Čárka: dlouhý blik LED
            digitalWrite(led_pin, HIGH);  // Rozsvícení LED
            delay(carka);           // Zpoždění podle délky čárky
            digitalWrite(led_pin, LOW);   // Zhasnutí LED
            Serial.print(" -");           // Výpis čárky na sériový monitor
        }
        // Pauza mezi jednotlivými symboly v rámci jednoho písmena
        if (i < length - 1) {
            delay(pauza);
        }
    }
    Serial.print(" /");  // Výpis oddělovače mezi písmeny na sériový monitor
}

// Funkce pro převod vstupního řetězce na Morseův kód a jeho následné blikání LED
// Parametry:
// - input: Vstupní řetězec, který obsahuje znaky k převodu na Morseův kód
void prevod(String input) {
    input.toUpperCase();  // Převod vstupu na velká písmena (Morseova abeceda pracuje s velkými písmeny)
    for (unsigned int i = 0; i < input.length(); i++) {  // Iterace přes jednotlivé znaky vstupu
        char letter = input[i];  // Získání aktuálního znaku
        if (letter == ' ') {
            // Pokud je znak mezera, vlož pauzu mezi slovy
            delay(pauzslov);
            Serial.print(" /");  // Výpis oddělovače mezi slovy na sériový monitor
        } else if (letter >= 'A' && letter <= 'Z') {
            // Pokud je znak písmeno (A-Z), najdi jeho index v Morseově abecedě
            int index = letter - 'A';  // Výpočet indexu písmena (A = 0, B = 1, ...)
            // Převod písmena na Morseův kód a blikání LED
            ledka(morseovka[index], pocet[index]);
            delay(pispauza);  // Pauza mezi jednotlivými písmeny
        } else {
            // Pokud je znak neplatný (není písmeno ani mezera), vypiš chybu a ukonči funkci
            Serial.println("Neplatný znak, použijte pouze písmena.");
            return;
        }
    }
    Serial.println("//");  // Výpis ukončení řetězce Morseova kódu na sériový monitor
}

// Hlavní smyčka programu, která běží opakovaně
void loop() {
    // Kontrola, zda byl zadán vstup přes sériový monitor
    if (Serial.available() > 0) {
        // Čtení vstupu jako řetězce
        String input = Serial.readString();
        Serial.println("Vstup: " + input);  // Výpis vstupu na sériový monitor
        // Převod vstupu na Morseův kód a jeho blikání pomocí LED
        prevod(input);
    }
}
