//Externe Bibliotheken laden
//
#include <Stepper.h> // Schrittmotor
#include <Wire.h> // I2C fuer Echtzeituhr
#include "RTClib.h" // Echtzeituhr


//Variabeln
//
const int SchliessZeitStunde = 22;
const int SchliessZeitMinute = 0;

const int OeffnungsZeitStunde = 6;
const int OeffnungsZeitMinute = 0;


byte state = 1; //offen 1, oeffnen 2, geschlossen 3, schliessen 4, fehler 5

const int maxStepperMillis = 180 * 1000; // maximale Motorlaufzeit in ms



unsigned long currentMillis = millis();
unsigned long lastLEDTime = currentMillis;
unsigned long lastStateTime = currentMillis;
unsigned long stepperStartMillis = currentMillis;

//PIN belegung
//
const byte pinStepperIN1 = 8; // Stepper
const byte pinStepperIN2 = 10;
const byte pinStepperIN3 = 9;
const byte pinStepperIN4 = 11;

const byte pinLEDSensorUnten = 4;
const byte pinLEDSensorOben = 5;

const byte pinSensorUnten = 3;
const byte pinSensorOben = 2;

const byte pinManuell = 6;
const byte pinManuellOeffnen = 7;


//Echtzeituhr eventuell loeschen ?
//
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif
RTC_PCF8523 rtc;

//Schritmotor
//
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, pinStepperIN1, pinStepperIN2, pinStepperIN3, pinStepperIN4);





//
//
void setup()
{
  myStepper.setSpeed(15);

  #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
  #endif

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }


  pinMode(pinSensorUnten, INPUT_PULLUP);
  pinMode(pinSensorOben, INPUT_PULLUP);
  pinMode(pinManuell, INPUT_PULLUP);
  pinMode(pinManuellOeffnen, INPUT_PULLUP);
  
  pinMode(pinLEDSensorUnten, OUTPUT);
  pinMode(pinLEDSensorOben, OUTPUT);
}





void loop() {
  currentMillis = millis();

  if(digitalRead(pinManuell) == LOW) { // Manueller modus
    
    if(digitalRead(pinManuellOeffnen) == HIGH) { state = 4; } //schliessen
    else { state = 2; } //oeffnen
    
    stepperStartMillis = currentMillis;
  }


  if(currentMillis - lastStateTime > 900)
  {
    lastStateTime = currentMillis;
    
    switch(state) {
      case 1:
        offen();
        break;
        
      case 2:
        oeffnen();
        break;
        
      case 3:
        geschlossen();
        break;
  
      case 4:
        schliessen();
        break;
        
      case 5:
        fehler();
        break;
    }
  }

  //LED Status Anzeige
  if(currentMillis - lastLEDTime > 50)
  {  
    lastLEDTime = currentMillis;
    LEDAnzeige();
  }
}
