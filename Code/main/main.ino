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

unsigned long date_time_last_update_time_point;
#include "date_time_struct.hpp"
#include "system_main.hpp"

// *** OPERATION SYSTEM ***

#include "blink.hpp"
#include "command_parser.hpp"
#include "events_manager.hpp"
#include "init.hpp"
#include "message_generator.hpp"
#include "print_debug.hpp"
#include "pars_request_struct.hpp"
#include "return_codes.hpp"
#include "serial_helper.hpp"

// only millis
unsigned long system_update_time_point;
unsigned long system_update_period = 60000; // 60 seconds
unsigned long date_time_update_time_point;
unsigned long date_time_update_delay;

struct system_info global_system_info = {0};

void fatalError() {
  printError("FATAL ERROR\n");

  pinMode(13, OUTPUT);
  while (1) {
    blinkAndWait_alarm();
  }
}

void systemSetup(struct system_info& result_system_info) {
  result_system_info.sim800_result = updateDateTime(global_date_time, sim800);
  if (result_system_info.sim800_result == SUCCESS) {
    // update at 0:00 am
    date_time_last_update_time_point = millis();
    date_time_update_time_point = date_time_last_update_time_point + getSecondsToNextDay(global_date_time) * 1000LU;
  } else {
    // try update after one minute
    date_time_update_time_point = millis() + 60000LU;
  }

  systemUpdateSendTime(result_system_info);

  system_update_time_point = millis();
}

void eventsFromSIM800(struct system_info& result_system_info) {
  if (sim800.available()) {
    printDebug(F("eventsFromSIM800: unparsed data begin\n"));
    while (sim800.available()) {
      char val = sim800.read();
#ifdef SERIAL_DEBUG
      Serial.write(val);
#endif // SERIAL_DEBUG
    }
    printDebug(F("eventsFromSIM800: unparsed data end\n"));

    printDebug(F("eventsFromSIM800: Event by sim800 response.\n"));

    system_update_time_point = millis();
    result_system_info.sim800_result = system_main_action(global_system_info);
  }
}

void eventsFromSerial(struct system_info& result_system_info) {
  struct ParsRequest request = {0};
  result_system_info.serial_result = parsRequestFrom(Serial, request);
  if (result_system_info.serial_result == SUCCESS) {
    printRequest(request, Serial);
    result_system_info.serial_result = doRequestAsSerial(request, global_system_info);
  }

  switch (result_system_info.serial_result) {
  case SUCCESS:
    printDebug(F("eventsFromSerial: SUCCESS\n"));
    break;
  case ERROR:
    printDebug(F("eventsFromSerial: ERROR\n"));
    break;
  case NO_REQUEST:
    blinkAndWait_2();
    break;
  default:
    printDebug(F("eventsFromSerial: unrecognise response "));
    printDebugInLine(result_system_info.serial_result);
    printDebugInLine('\t');
    printDebugInLine(SUCCESS);
    printDebugInLine('\n');

    blinkAndWait_4();
    break;
  }
}

void eventsFromSystem(struct system_info& result_system_info) {
  if (eventAvailable(system_update_time_point) == SUCCESS) {
    system_update_time_point = millis() + system_update_period;
    printDebug(F("eventsFromSystem: Every 60 seconds event.\n"));
    result_system_info.sim800_result = system_main_action(global_system_info);
  }

  if (result_system_info.sim800_result == ERROR) {
    result_system_info.sim800_result = system_fix_action();
  }

  if (result_system_info.send_measured_data && eventAvailable(result_system_info.send_measured_data_time) == SUCCESS) {
    setNextDay(result_system_info.send_measured_data_time);
    struct ParsRequest request = {0};
    
    printDebug(F("eventsFromSystem: do PRINT_MEASURED_DATA\ndate_time: "));
    printDateTime(result_system_info.send_measured_data_time, Serial);
    
    request.commands_list |= PRINT_MEASURED_DATA;
    result_system_info.sim800_result = doRequestAsSIM800(request, global_system_info);
  }

  if (eventAvailable(date_time_update_time_point, 3600LU * 24 * 4 * 1000) == SUCCESS) {
    result_system_info.sim800_result = updateDateTime(global_date_time, sim800);
    if (result_system_info.sim800_result == SUCCESS) {
      // update at 0:00 am
      date_time_last_update_time_point = millis(); // must be for getCurrentTimeInSeconds
      date_time_update_time_point = date_time_last_update_time_point + getSecondsToNextDay(global_date_time) * 1000LU;
    } else {
      // try update after one minute
      date_time_update_time_point = millis() + 60000LU;
    }
  }

  return;
}

void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
#endif // SERIAL_DEBUG
  sim800.begin(9600);

  printDebug(F("setup: Begin\n"));

  return_code_t return_code = init(10); // one attemp arout 10 seconds -> 100 seconds until timeout
  
  dht.begin();

  if (return_code != SUCCESS) {
    fatalError();
  }

  float h = dht.readHumidity();
  if (h != h) {
    printWarning(F("main: Temperature sensor initialisation failed.\n"));
    // fatalError();
  }

  systemSetup(global_system_info);

  printDebug(F("setup: End\n"));
}

void loop() {
  eventsFromSIM800(global_system_info);
  eventsFromSerial(global_system_info);
  eventsFromSystem(global_system_info);
}














