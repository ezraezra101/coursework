int readMotion() {
  return digitalRead(MOTION_PIN);
}

int readLight() {
  // Circuit is: 5V -> Photoresistor -> (A2) -> 51KOhms resistor -> Ground
  
  int light = analogRead(LIGHT_PIN);
  // Assumes connected to 5V
  const float maxLight = 1023;
  
  const float lowerThreshold = 0.5;
  const float upperThreshold = 0.6;
  
  static int lastLight = LOW;

  if(light < maxLight * lowerThreshold) {
    lastLight = LOW;
  } else if(light > maxLight * upperThreshold) {
    lastLight = HIGH;
  }

  return lastLight;
}


float readTemp() {
  // Assuming that we don't change things with analogReference
  // Arduino reads between 0V (0) and 5V (1023)

  // Temperature sensor specs:
  // 2.7V to 5.5V Vin
  // Output range: 0.1V (-40C), 2V (150C)
  // 2C accuracy
  float reading = analogRead(TEMP_PIN);
  reading = reading - 20; //FUDGE FACTOR
  // Convert to voltage
  reading = (reading / 1024) * 5;

  //Convert to degrees C
  // Range = 1.9V, or 190C
  reading = ((reading - 0.1) * (190/1.9)) - 40;
  return reading;
}

// Reads the read switch.
// 0 for not connected 1 for connected
int readReed() {
  return !digitalRead(REED_PIN);
}
