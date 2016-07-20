void oeffnen() { // state 2

  if(digitalRead(pinSensorOben) == HIGH) {
    
    if((currentMillis - stepperStartMillis) < maxStepperMillis) {
      
      myStepper.step(stepsPerRevolution*15/60); // Stepper 1 s laufen lassen
    }
    
    else {
      state = 5; //Fehler
    }
  }
  
  else {
    state = 1; // Offen
  }
  
  // Motor ausschalten
  motorAus();
}



void offen() { // state 1
  
  if(digitalRead(pinManuell) == HIGH and ((rtc.now().hour() >= SchliessZeitStunde and rtc.now().minute() >= SchliessZeitMinute) or (rtc.now().hour() < OeffnungsZeitStunde and rtc.now().minute() < OeffnungsZeitMinute))) {
    stepperStartMillis = currentMillis;
    state = 4; //schliessen
  }
}

void schliessen() { // state 4
  if(digitalRead(pinSensorUnten) == HIGH) {

    if(currentMillis - stepperStartMillis < maxStepperMillis) {

      myStepper.step(-stepsPerRevolution*15/60); // Stepper 1 s laufen lassen
    }
    
    else {
      state = 5; // Fehler
    }
  }
  else {
    state = 3; // geschlossen
  }
  
  // Motor ausschalten
  motorAus();
}




void geschlossen() { // state  3

  if((digitalRead(pinManuell) == HIGH) and (rtc.now().hour() >= OeffnungsZeitStunde and rtc.now().minute() >= OeffnungsZeitMinute) and (rtc.now().hour() < SchliessZeitStunde and rtc.now().minute() < SchliessZeitMinute)) {
    stepperStartMillis = currentMillis;
    state = 2; // oeffnen
  }
}



void fehler() { // state 5
  // Motor ausschalten
  motorAus();
}


void motorAus(){
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
}

// Dient zur viesuellen Anzeige per LED ob ein MagnetSchalter aktiviert ist.

// Der MagnetSchalter hat einen Kontakt mit GND verbunden und den anderen mit dem Pin pinSensorUnten/pinSensorOben.
// Der Pin pinSensorUnten/pinSensorOben ist standardmaessig HIGH (INPUT_PULLUP)
// Falls ein Magnet beim Sensor ist, verbindet dieser die beiden Kontakte.
// Also: pdigitalRead(inSensorUnten) == LOW bedeutet, dass das Magnet beim Sensor ist.
void LEDAnzeige() {
  
  if(digitalRead(pinSensorUnten) == LOW) { //Magnet Sensor Unten
    digitalWrite(pinLEDSensorUnten, HIGH);
  }
  else {
    digitalWrite(pinLEDSensorUnten, LOW);
  }

  
  if(digitalRead(pinSensorOben) == LOW) { //Magnet Sensor Oben
    digitalWrite(pinLEDSensorOben, HIGH);
  }
  else {
    digitalWrite(pinLEDSensorOben, LOW);
  }
}
