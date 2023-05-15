#define SERIAL_DEBUG

// *** SIM800 ***

#include <SoftwareSerial.h>
enum SIM800_WIRES {
  SIM800_RX = 11,
  SIM800_TX = 12
};
SoftwareSerial sim800(SIM800_TX, SIM800_RX);

// *** DHT ***

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

// *** OPERATION SYSTEM ***

#include "blink.hpp"
#include "command_parser.hpp"
#include "init.hpp"
#include "print_debug.hpp"
#include "return_codes.hpp"

void fatalError() {
  printError("FATAL ERROR\n");

  pinMode(13, OUTPUT);
  while (1) {
    blinkAndWait_alarm();
  }
}

struct parsCondition pars_conditional;


void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif // SERIAL_DEBUG
  sim800.begin(9600);

  return_code_t return_code = init(-1, pars_conditional);
  
  dht.begin();

  if (return_code != SUCCESS) {
    fatalError();
  }

  float h = dht.readHumidity();
  if (h != h) {
    printError("main: Temperature sensor initialisation failed.");
    fatalError();
  }

  Serial.println();
}

void loop() {
  bool some_go = false;
  while (sim800.available()) {
    char val = sim800.read();
    Serial.write(val);
  }

#ifdef LAST
  while (Serial.available()) {
    parsInput(pars_conditional, Serial.read());
  }
#else   // LAST
  return_code_t result = parsRequestFromSerial();
  switch (result) {
  case SUCCESS:
    printDebug("main: SUCCESS\n");
    break;
  case ERROR:
    printDebug("main: ERROR\n");
    break;
  case NO_REQUEST:
    blinkAndWait_2();
    break;
  default:
    printDebug("main: unrecognise response ");
    printDebugInLine(result);
    printDebugInLine('\t');
    printDebugInLine(SUCCESS);
    printDebugInLine('\n');

    blinkAndWait_4();
    break;
  }
#endif  // LAST
}














