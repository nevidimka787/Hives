#pragma once

#include <SoftwareSerial.h>

#include "commands_list.hpp"
#include "eeprom_commands.hpp"
#include "date_time_struct.hpp"
#include "print_debug.hpp"
#include "pars_request_struct.hpp"
#include "return_codes.hpp"
#include "serial_helper.hpp"
#include "SMS_parser.hpp"
#include "str_macroses.hpp"
#include "system_info_struct.h"

enum {
  NOTHING_DETECTED = 0,
  O_DETECTED = 1,
  K_DETECTED = 2,
  NEW_LINE_DETECTED = 3,
  RETURN_DETECTED = 4
};

typedef enum {
  INPUT_COMM_REPEAT,
  HEAD,
  MESSAGE
} checking_state_t;

typedef enum {
  CMGL,
  INDEX,
  STAT,
  PHONE_NUMBER,
  ALPHA,
  DATA_TIME,
  OUT_OF_HEAD 
} position_state_t;

struct SMS_pars_conditionals {
  checking_state_t checking_state;
  position_state_t position_state;

  int current_sms_number;
  
  bool sms_valied_valid;
};

return_code_t printUntilOk(Stream& check_serial) {
  char c;
  while(1) {
    if (waitAvailable(check_serial) != SUCCESS) { return ERROR; }

    Serial.print(c = check_serial.read());
    
    if (c != 'O') { continue; }
    if (waitAvailable(check_serial) == SUCCESS) { c = check_serial.read(); }
    Serial.print(c);
    if (c != 'K') { continue; }
    if (waitAvailable(check_serial) == SUCCESS) { c = check_serial.read(); }
    Serial.print(c);
    if (c != '\r') { continue; }
    if (waitAvailable(check_serial) == SUCCESS) { c = check_serial.read(); }
    Serial.print(c);
    if (c != '\n') { continue; }
    if (waitAvailable(check_serial) != SUCCESS) {
      return SUCCESS;
    }
  }
}

return_code_t checkSim800OK(int times = 100, Stream& sim800_serial = sim800) {
  //#define printUntilOk_AS_checkSim800OK
  #ifndef printUntilOk_AS_checkSim800OK
  char c;
  while(1) {
    if (waitAvailable(sim800_serial, times) != SUCCESS) { break; }

    c = sim800_serial.read();
    
    if (c != 'O') { continue; }
    if (waitAvailable(sim800_serial, times) == SUCCESS) { c = sim800_serial.read(); }
    if (c != 'K') { continue; }
    if (waitAvailable(sim800_serial, times) == SUCCESS) { c = sim800_serial.read(); }
    if (c != '\r') { continue; }
    if (waitAvailable(sim800_serial, times) == SUCCESS) { c = sim800_serial.read(); }
    if (c != '\n') { continue; }
    if (waitAvailable(sim800_serial, times) != SUCCESS) {
      return SUCCESS;
    }
  }
  
  printError(F("checkSim800OK: invalid response\n"));
  return ERROR;
  #else // printUntilOk_AS_checkSim800OK
  return_code_t result = printUntilOk(sim800_serial);
  if (result != SUCCESS) {
    printError(F("checkSim800OK (as printUntilOk): invalid response\n"));
  }
  return result;
  #endif //printUntilOk_AS_checkSim800OK
}

/*
Get number of sms from serial.
Delete sms with selected number.
*/
return_code_t deleteSMS(Stream& serial_with_number, Stream& request_to_serial = sim800) {
  request_to_serial.print(F("AT+CMGD="));
  request_to_serial.println(Serial.parseInt());

  return checkSim800OK();
}

/*
Delete sms with selected number.
*/
return_code_t deleteSMS(int sms_number, Stream& request_to_serial = sim800) {
  request_to_serial.print(F("AT+CMGD="));
  request_to_serial.println(sms_number);

  return checkSim800OK();
}

return_code_t deleteSMSAll(Stream& request_to_serial = sim800) {
  request_to_serial.println(F("AT+CMGDA=\"DEL ALL\""));
  return checkSim800OK();
}

return_code_t Sim800Check(int response_delay = 1000) {
  sim800.println(F("AT"));

  delay(response_delay);
  return checkSim800OK();
}

return_code_t Sim800Config(int arg = 1, int response_delay = 100) {
  sim800.print(F("AT+CMGF="));
  sim800.println(arg);
  delay(response_delay);
  if (checkSim800OK() != SUCCESS) {
    return ERROR;
  }

  sim800.println(F("AT+CSCS=\"GSM\""));
  delay(response_delay);
  if (checkSim800OK() != SUCCESS) {
    return ERROR;
  }

  sim800.println(F("AT+CLTS=1;&W"));
  delay(response_delay);
  if (checkSim800OK() != SUCCESS) {
    return ERROR;
  }
}

return_code_t printAllSMSToSerial() {
  sim800.println(F("AT+CMGL=\"ALL\""));
 
  return printUntilOk(sim800);
}

return_code_t getLastSMSId(int& last_sms_id) {
  sim800.println(F("AT+CMGL=\"ALL\""));

  last_sms_id = getNewestSMSFromNumberUntilOK(sim800);

  printDebug(F("getLastSMSId: sms_number: "));
  printDebugInLine(last_sms_id);
  printDebugInLine('\n');
  return last_sms_id == 0 ? ERROR : SUCCESS;
}

return_code_t printSMSToSerial(int sms_number) {
  sim800.print(F("AT+CMGR="));
  sim800.println(sms_number);
  
  Serial.println(F("printSMSToSerial:"));

  return printUntilOk(sim800);
}

return_code_t printMeasuredDataTo(Stream& serial) {
  serial.print(F("printMeasuredDataToSerial:\nHumidity: "));
  serial.println(dht.readHumidity());
  serial.print(F("Temperature: "));
  serial.println(dht.readTemperature());

  return SUCCESS;
}

return_code_t shortPrintMeasuredDataTo(Stream& serial) {
  serial.print(F("Environmental data\nTemperature: "));
  serial.print(dht.readTemperature());
  serial.print(F(" C\nHumidity: "));
  serial.print(dht.readHumidity());
  serial.println(F(" %"));

  return SUCCESS;
}

return_code_t updateDateTime(struct date_time& date_time_i, Stream& data_source_serial = sim800) {
  data_source_serial.println(F("AT+CCLK?"));
  
  while (waitAvailable(data_source_serial) == SUCCESS) {
    char c = data_source_serial.read();
    if (c == '\"') {
      if (scanDateTime(date_time_i, data_source_serial) != SUCCESS) {
        return ERROR;
      }

#ifdef SERIAL_DEBUG
      printDateTime(date_time_i, Serial);
#endif

      return checkSim800OK();
    }
  }

  return ERROR;
}

return_code_t setSendTime(struct date_time& date_time, struct system_info& system_info) {
  setSendTime(date_time);
  system_info.set_send_time = true;
  return SUCCESS;
}

















