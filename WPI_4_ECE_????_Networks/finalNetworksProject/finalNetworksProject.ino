#include <RadioHead.h>
#include <RH_ASK.h>
#include <SPI.h>

class Radio: public RH_ASK {
  public:
    Radio(uint16_t speed, uint8_t rxPin, uint8_t txPin, uint8_t pttPin, bool pttInverted)
        : RH_ASK(speed, rxPin, txPin, pttPin, pttInverted)
    {
    }

  
    void setRxInverted(bool i) {
      _rxInverted = i;
    }
};

typedef struct {
  uint8_t device_id;
  uint8_t information;
} packet;

typedef struct {
  long int last_received;
  bool valid;
  uint8_t information;
} deviceinfo;

// RF test sketch

#define LIGHT_SENSOR 0
#define MOTION_SENSOR 1
#define REED_SENSOR 2
#define TEMP_SENSOR 3

// Helpers for finding bitmasks for fields in packet.information and deviceinfo.information
#define PACKET_SENSOR_VALUE(sensor) ((uint8_t) 1 << ((sensor) * 2))
#define PACKET_SENSOR_ENABLE(sensor) ((uint8_t) 2 << ((sensor) * 2))


#define TX_PIN 3
#define RX_PIN A0//A0
#define LED_PIN 13
#define TEMP_PIN A1
#define REED_PIN 4
#define LIGHT_PIN A2
#define MOTION_PIN 2
#define HEATER_PIN 5 // Arbitrarily chosen and not used yet.

#define DEVICE_ID 1

#define LIGHT_ATTACHED true
#define MOTION_ATTACHED false
#define REED_ATTACHED false
#define TEMP_ATTACHED false


//List of information from devices
#define NUM_SENSORS 4 // Really number of devices, including AP
deviceinfo current_state[NUM_SENSORS];


Radio driver(2000, RX_PIN, TX_PIN, 5, false); // ESP8266: do not use pin 11

packet constructPacket() {
  packet p;
  p.device_id = DEVICE_ID;
  p.information = 0;

  // Light sensor
  if(LIGHT_ATTACHED) {
    uint8_t msg = 1 & (uint8_t)readLight();
    p.information |= PACKET_SENSOR_ENABLE(LIGHT_SENSOR);
    p.information |= msg ? PACKET_SENSOR_VALUE(LIGHT_SENSOR) : 0;
  }
  // Motion sensor
  if(MOTION_ATTACHED) {
    uint8_t msg = 1 & (uint8_t)readMotion();
    p.information |= 2 << (2 * MOTION_SENSOR);
    p.information |= msg << (2 * MOTION_SENSOR);
  }

  // Reed switch
  if(REED_ATTACHED) {
    uint8_t msg = 1 & (uint8_t)readReed();
    p.information |= 2 << (2 * REED_SENSOR);
    p.information |= msg << (2 * REED_SENSOR);
  }

  // Temperature sensor
  if(TEMP_ATTACHED) {
    uint8_t msg = 1 & (uint8_t)(readTemp() > 20);
    p.information |= 2 << (2 * TEMP_SENSOR);
    p.information |= msg << (2 * TEMP_SENSOR);
  }
  return p;  
}

void printPacketInfo(uint8_t info) {
  if(PACKET_SENSOR_ENABLE(MOTION_SENSOR) & info)
    { Serial.print(" MOTION sensor: "); Serial.print(!!(PACKET_SENSOR_VALUE(MOTION_SENSOR) & info)); }
  if(PACKET_SENSOR_ENABLE(REED_SENSOR) & info)
    { Serial.print(" REED sensor: "); Serial.print(!!(PACKET_SENSOR_VALUE(REED_SENSOR) & info)); }
  if(PACKET_SENSOR_ENABLE(LIGHT_SENSOR) & info)
    { Serial.print(" LIGHT sensor: "); Serial.print(!!(PACKET_SENSOR_VALUE(LIGHT_SENSOR) & info)); }
  if(PACKET_SENSOR_ENABLE(TEMP_SENSOR) & info)
    { Serial.print(" TEMP sensor: "); Serial.print(!!(PACKET_SENSOR_VALUE(TEMP_SENSOR) & info)); }
  Serial.println();
}

bool anyDoorsOpen() {
  for(int i=0; i<NUM_SENSORS; i++) { // For each sensor
    uint8_t info = current_state[i].information;
    if(info & PACKET_SENSOR_ENABLE(REED_SENSOR) && info & PACKET_SENSOR_VALUE(REED_SENSOR)
      ) {
        return true;
      }
  }
  return false;
}

bool detected(uint8_t sensor) {
    for(int i=0; i<NUM_SENSORS; i++) {
    uint8_t info = current_state[i].information;
    if(info & PACKET_SENSOR_ENABLE(sensor) && info & PACKET_SENSOR_VALUE(sensor)
    )
      return true;
  }
  return false;
}
//Have all of the sensors reported in in the last 30 minutes?
bool validateState() {
  bool allValid = true;
  for(int i=1; i<NUM_SENSORS; i++) { // For all non-base station sensors

  //Example:
  // millis/1000 = 1
  // last_rec = 10000
  // Overflows at 1111
  // last_rec - 1111 
    
    if(millis()/1000 - current_state[i].last_received > 1800) { // We need to do this better
      current_state[i].valid = false;
    }
    if(!current_state[i].valid) {
      allValid = false;
    }
  }
  return allValid;
}

bool highTemp() {
  return readTemp() > 20.0;
}

void heaterOn() {
  Serial.println("Heater on");
  digitalWrite(HEATER_PIN, HIGH);
}
void heaterOff() {
  Serial.println("Heater off");
  digitalWrite(HEATER_PIN, LOW);
}

void updateHeater() {
  // Note: if we loose connection with any of the boards, we want to either notify the person that something is wrong or start ignoring it
  // In a way that allows us to still make decisions based on remaining inputs.

  // If any of our sensors haven't talked to us in 30 minutes, shut off heater!
  if(!validateState()) {
    heaterOff();
    Serial.println("One of the sensors is not transmitting");
  }
  
  if(anyDoorsOpen() || highTemp()) {
    heaterOff();
  } else if(detected(MOTION_SENSOR) || detected(LIGHT_SENSOR)) { // If you're working at a desk, you might not be moving, but the lights are probably on.
    heaterOn();
  } else {
    heaterOff();
  }
}

void callback(packet p) {
  Serial.print("Received packet from device ");Serial.print(p.device_id);
  printPacketInfo(p.information);

  current_state[p.device_id].last_received = millis()/1000;
  current_state[p.device_id].information = p.information;
  current_state[p.device_id].valid = true;

  updateHeater();
}

void setup() {
  pinMode(TX_PIN, OUTPUT);     
  pinMode(LED_PIN, OUTPUT);
  pinMode(REED_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  if (!driver.init())
         Serial.println("init failed");
  driver.setRxInverted(false);
}

void loop() {
  if(Serial.available()) {
    Serial.read();
    Serial.println(DEVICE_ID);
  }
  
  if(DEVICE_ID == 0) {
    listen(callback); // Non-blocking call

    // Update heater about every 10 seconds... (and in callback)
    static int lastUpdate = 0;
    int m = millis()-lastUpdate;
    if(m < 0) { lastUpdate = 0; m = 0; }
    if(m > 10000) {
      lastUpdate = millis();
      updateHeater();
    }
    
  } else {
    packet p = constructPacket();
    Serial.print(DEVICE_ID); printPacketInfo(p.information);
    transmit(p);
    delay(1000);
  }
}
