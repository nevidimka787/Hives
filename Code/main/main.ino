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
#include "message_generator.hpp"
#include "print_debug.hpp"
#include "pars_request_struct.hpp"
#include "return_codes.hpp"
#include "serial_helper.hpp"
#include "system_main.hpp"

// only millis
unsigned long system_update_time_point;
unsigned long system_update_period = 60000; // 60 seconds
unsigned long date_time_update_time_point;
unsigned long date_time_update_delay;

struct date_time global_date_time;

struct system_info global_system_info = {0};

return_code_t serial_result;
return_code_t sim800_result;

void fatalError() {
  printError("FATAL ERROR\n");

  pinMode(13, OUTPUT);
  while (1) {
    blinkAndWait_alarm();
  }
}

void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
#endif // SERIAL_DEBUG
  sim800.begin(9600);

  printDebug(F("setup: Begin\n"));

  return_code_t return_code = init(
    10,
    global_date_time,
    date_time_update_delay); // one attemp arout 10 seconds -> 100 seconds until timeout
  
  dht.begin();

  if (return_code != SUCCESS) {
    fatalError();
  }

  float h = dht.readHumidity();
  if (h != h) {
    printWarning(F("main: Temperature sensor initialisation failed.\n"));
    // fatalError();
  }

  printDebug(F("setup: End\n"));

  system_update_time_point = millis();
  date_time_update_time_point = millis();
}

void loop() {
  struct ParsRequest request = {0};
  if (sim800.available()) {
    printDebug(F("main: unparsed data begin\n"));
    while (sim800.available()) {
      char val = sim800.read();
#ifdef SERIAL_DEBUG
      Serial.write(val);
#endif // SERIAL_DEBUG
    }
    printDebug(F("main: unparsed data end\n"));

    printDebug(F("main: Event by sim800 response.\n"));

    system_update_time_point = millis();
    sim800_result = system_main_action(global_system_info);
  }

  return_code_t serial_result = parsRequestFrom(Serial, request);
  if (serial_result == SUCCESS) {
    printRequest(request, Serial);
    serial_result = doRequestAsSerial(request);
  }

  switch (serial_result) {
  case SUCCESS:
    printDebug(F("main: SUCCESS\n"));
    break;
  case ERROR:
    printDebug(F("main: ERROR\n"));
    break;
  case NO_REQUEST:
    blinkAndWait_2();
    break;
  default:
    printDebug(F("main: unrecognise response "));
    printDebugInLine(serial_result);
    printDebugInLine('\t');
    printDebugInLine(SUCCESS);
    printDebugInLine('\n');

    blinkAndWait_4();
    break;
  }

  if (millis() - system_update_time_point >= system_update_period) {
    system_update_time_point = millis();

    printDebug(F("main: Every 60 seconds event.\n"));
    
    sim800_result = system_main_action(global_system_info);
  }

  if (sim800_result == ERROR) {
    sim800_result = system_fix_action();
  }

  if (millis() - date_time_update_time_point >= date_time_update_delay) {
    sim800_result = updateDateTime(global_date_time, sim800);
    if (sim800_result == SUCCESS) {
      // update at 1:00 am
      date_time_update_delay = getdateTimeUpdateDelay(global_date_time);
      date_time_update_time_point = millis();
    }
  }
}














