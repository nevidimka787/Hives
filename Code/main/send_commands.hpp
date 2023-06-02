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

// send AT to sim800 and wait OK
// @param timeout - time (in millisecinds) after that the function returns result
// @param sim800_serial - stream checking by the function
// @return SUCCES if the stream work correctly, else ERROR
return_code_t checkSim800OK(int timeout = 1000, Stream& sim800_serial = sim800);

// Get number of SMS from serial.
// Delete sms with selected number.
// @param serial_with_number - input stream
// @param request_to_serial - stream that support AT commands
// @return SUCCESS if SMS was deletet else ERROR
return_code_t deleteSMS(Stream& serial_with_number, Stream& request_to_serial = sim800);

// Delete SMS with selected number.
// @param sms_number - number of SMS
// @param request_to_serial - stream that support AT commands
// @return SUCCESS if SMS was deletet else ERROR
return_code_t deleteSMS(int sms_number, Stream& request_to_serial = sim800);

// Delete all SMS.
// @param request_to_serial - stream that support AT commands
// @return SUCCESS if all SMS were deletet else ERROR
return_code_t deleteSMSAll(Stream& request_to_serial = sim800);

// get id of the last SMS resived frome user's phone number
// @param last_sms_id - SMS id
// @return SUCCESS if SMS was found else ERROR
return_code_t getLastSMSId(int& last_sms_id);

// @return always SUCCESS
return_code_t printAllSMSToSerial();

// @param sms_number - number of SMS
// @return SUCCESS if SMS was fprinted else ERROR
return_code_t printSMSToSerial(int sms_number);

// print enviromental data that have been measured
// format: debug
// @param serial- output stream
// @return alwayse SUCCESS
return_code_t printMeasuredDataTo(Stream& serial);

// send AT to sim800 and wait OK
// print all output from sim800 (include OK)
// @param timeout - time (in millisecinds) after that the function returns result
// @param sim800_serial - stream checking by the function
// @param serial - stream for printing
// @return SUCCES if the stream work correctly, else ERROR
return_code_t printUntilOk(int timeout = 1000, Stream& sim800_serial = sim800, Stream& serial = Serial);

// print enviromental data that have been measured
// format: user friendly
// @param serial- output stream
// @return alwayse SUCCESS
return_code_t shortPrintMeasuredDataTo(Stream& serial);

// send AT and wait OK
// @param timeout - millisecond to error
// @return SUCCESS if sim800 work correctly, else ERORR
return_code_t Sim800Check(int timeout = 1000);

// configure sim800
// @param timeout - timeout per operation until ERROR
return_code_t Sim800Config(int timeout = 1000);

// @param date_time - time of sending SMS
// @param system_info - system information
// @return always SUCCESS
return_code_t setSendTime(const struct date_time& date_time, struct system_info& system_info);

// @param date_time_i - updated date time will be writed to here
// @param data_source_serial - stream that provide the time and date
return_code_t updateDateTime(struct date_time& date_time_i, Stream& data_source_serial = sim800);

return_code_t printUntilOk(int timeout, Stream& sim800_serial, Stream& serial) {
  char c;
  while(1) {
MAIN_CONTINUE:
    if (waitAvailable(sim800_serial, timeout) != SUCCESS) {
      printError("checkSim800OK: timeout: ");
      printDebugInLine(timeout);
      printDebugInLine('\n');
      break;
    }

    c = sim800_serial.read();
    serial.print(c);
    
    if (c != 'O') { continue; }
    if (waitAvailable(sim800_serial, timeout) == SUCCESS) {
      c = sim800_serial.read();
      serial.print(c);
    }
    if (c != 'K') { continue; }
    while (waitAvailable(sim800_serial, timeout) == SUCCESS) {
      c = sim800_serial.read();
      serial.print(c);
      if (!isSpaceSymbol(c)) {
        goto MAIN_CONTINUE;
      }
    }
    return SUCCESS;
  }
  
  printError(F("printUntilOk: invalid response\n"));
  return ERROR;
}

return_code_t checkSim800OK(int timeout, Stream& sim800_serial) {
  //#define printUntilOk_AS_checkSim800OK
  #ifndef printUntilOk_AS_checkSim800OK
  char c;
  while(1) {
MAIN_CONTINUE:
    if (waitAvailable(sim800_serial, timeout) != SUCCESS) {
      printError("checkSim800OK: timeout: ");
      printDebugInLine(timeout);
      printDebugInLine('\n');
      break;
    }

    c = sim800_serial.read();
    
    if (c != 'O') { continue; }
    if (waitAvailable(sim800_serial, timeout) == SUCCESS) { c = sim800_serial.read(); }
    if (c != 'K') { continue; }
    while (waitAvailable(sim800_serial, timeout) == SUCCESS) {
      c = sim800_serial.read();
      if (!isSpaceSymbol(c)) {
        goto MAIN_CONTINUE;
      }
    }
    return SUCCESS;
  }
  
  printError(F("checkSim800OK: invalid response\n"));
  return ERROR;
  #else // printUntilOk_AS_checkSim800OK
  return_code_t result = printUntilOk(timeout, sim800_serial);
  if (result != SUCCESS) {
    printError(F("checkSim800OK (as printUntilOk): invalid response\n"));
  }
  return result;
  #endif //printUntilOk_AS_checkSim800OK
}

return_code_t deleteSMS(Stream& serial_with_number, Stream& request_to_serial = sim800) {
  request_to_serial.print(F("AT+CMGD="));
  request_to_serial.println(Serial.parseInt());

  return checkSim800OK();
}

return_code_t deleteSMS(int sms_number, Stream& request_to_serial = sim800) {
  request_to_serial.print(F("AT+CMGD="));
  request_to_serial.println(sms_number);

  return checkSim800OK();
}

return_code_t deleteSMSAll(Stream& request_to_serial = sim800) {
  request_to_serial.println(F("AT+CMGDA=\"DEL ALL\""));
  return checkSim800OK();
}

return_code_t Sim800Check(int timeout) {
  sim800.println(F("AT"));

  return checkSim800OK(timeout);
}

return_code_t Sim800Config(int timeout) {
  printDebug(F("Sim800Config: timeout: "));
  printDebugInLine(timeout);
  printDebugInLine('\n');

  sim800.println(F("AT+CMGF=1"));

  if (checkSim800OK(timeout) != SUCCESS) {
    printDebug(F("Sim800Config: AT+CMGF=1\n"));
    return ERROR;
  }

  sim800.println(F("AT+CSCS=\"GSM\""));
  if (checkSim800OK(timeout) != SUCCESS) {
    printDebug(F("Sim800Config: AT+CSCS=\"GSM\"\n"));
    return ERROR;
  }

  sim800.println(F("AT+CLTS=1;&W"));
  if (checkSim800OK(timeout) != SUCCESS) {
    printDebug(F("Sim800Config: AT+CLTS=1;&W\n"));
    return ERROR;
  }

  printDebug(F("Sim800Config: success\n"));
  return SUCCESS;
}

return_code_t printAllSMSToSerial() {
  sim800.println(F("AT+CMGL=\"ALL\""));
 
  return printUntilOk(1000, sim800);
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

  return printUntilOk(1000, sim800);
}

return_code_t printMeasuredDataTo(Stream& serial) {
  serial.print(F("printMeasuredDataToSerial:\nHumidity: "));
  serial.println(dht.readHumidity());
  serial.print(F("Temperature: "));
  serial.println(dht.readTemperature());
  serial.print(F("Weight: "));
  serial.println(scale.read());

  return SUCCESS;
}

return_code_t shortPrintMeasuredDataTo(Stream& serial) {
  serial.print(F("Environmental data\nWeight: "));
  serial.print(scale.read());
  serial.print(F(" <no units>\nTemperature: "));
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

return_code_t setSendTime(const struct date_time& date_time, struct system_info& system_info) {
  setSendTime(date_time);
  system_info.set_send_time = true;
  return SUCCESS;
}

















