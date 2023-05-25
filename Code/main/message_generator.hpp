#pragma once

#include <Stream.h>

#include "eeprom_commands.hpp"
#include "return_codes.hpp"
#include "send_commands.hpp"

using namespace return_codes;

return_code_t sendDefaultSMS(Stream& to_serial = sim800) {
  char phone_number[20];
  getPhoneNumber(phone_number);
  Serial.println(phone_number);
  to_serial.print(F("AT+CMGS=\"+"));
  to_serial.print(phone_number);
  to_serial.println(F("\""));
  
  waitAvailable(to_serial);
  to_serial.println(F("Default message from SIM800.\n"));
  to_serial.write((char)26);

  return checkSim800OK(10000);
}

return_code_t doRequestAsSerial(struct ParsRequest& request) {
  long return_codes = 0;
  if (request.commands_list & DELETE_SMS) {
    printDebug(F("parsRequest: deleteSms\n"));
    if (deleteSMS(request.sms_number) == SUCCESS) {
      return_codes |= DELETE_SMS;
    }
  }
  if (request.commands_list & CHECK_OK) {
    printDebug(F("parsRequest: Sim800Check\n"));
    if(Sim800Check() == SUCCESS) {
      return_codes |= CHECK_OK;
    }
  }
  if (request.commands_list & CMGF_EN) {
    printDebug(F("parsRequest: Sim800Config_CMGF\n"));
    if (Sim800Config_CMGF() == SUCCESS) {
      return_codes |= CMGF_EN;
    }
  }
  if (request.commands_list & GET_LAST_SMS_ID) {
    printDebug(F("parsRequest: getLastSMSId\n"));

    int temp;
    if (getLastSMSId(temp) == SUCCESS) {
      return_codes |= GET_LAST_SMS_ID;
    }
  }
  if (request.commands_list & SEND_SMS) {
    printDebug(F("parsRequest: sendSMSFromSerial\n"));
    if (sendDefaultSMS() == SUCCESS) {
      return_codes |= SEND_SMS;
    }
  }
  if (request.commands_list & SET_NUMBER) {
    printDebug(F("parsRequest: setPhoneNumber\n"));
    if (setPhoneNumber(request.phone_number) == SUCCESS) {
      return_codes |= SET_NUMBER;
    }
  }
  if (request.commands_list & SET_MAX_TEMPERATURE) {
    
    printDebug(F("parsRequest: setMaxTemperature\n"));
    if (setMaxTemperature(request.max_temperature) == SUCCESS) {
      return_codes |= SET_MAX_TEMPERATURE;
    }
  }
  if (request.commands_list & SET_MAX_HUMIDITY) {
    
    printDebug(F("parsRequest: setMaxHumidity\n"));
    if (setMaxHumidity(request.max_humidity) == SUCCESS) {
      return_codes |= SET_MAX_HUMIDITY;
    }
  }
  if (request.commands_list & SET_MIN_TEMPERATURE) {
    
    printDebug(F("parsRequest: setMinTemperature\n"));
    if (setMinTemperature(request.min_temperature) == SUCCESS) {
      return_codes |= SET_MIN_TEMPERATURE;
    }
  }
  if (request.commands_list & SET_MIN_HUMIDITY) {
    
    printDebug(F("parsRequest: setMinHumidity\n"));
    if (setMinHumidity(request.min_humidity) == SUCCESS) {
      return_codes |= SET_MIN_HUMIDITY;
    }
  }
  if (request.commands_list & PRINT_SMS) {
    printDebug(F("parsRequest: printSMSToSerial\n"));
    if (printSMSToSerial(request.sms_number) == SUCCESS) {
      return_codes |= PRINT_SMS;
    }
  }
  if (request.commands_list & PRINT_SMS_ALL) {
    printDebug(F("parsRequest: printAllSMSToSerial\n"));
    if (printAllSMSToSerial() == SUCCESS) {
      return_codes |= PRINT_SMS_ALL;
    }
  }
  if (request.commands_list & PRINT_STORED_DATA) {
    printDebug(F("parsRequest: printStoredDataTo\n"));
    if (printStoredDataTo(Serial) == SUCCESS) {
      return_codes |= PRINT_STORED_DATA;
    }
  }
  if (request.commands_list & PRINT_MEASURED_DATA) {
    printDebug(F("parsRequest: printMeasuredDataToSerial\n"));
    if (printMeasuredDataToSerial() == SUCCESS) {
      return_codes |= PRINT_MEASURED_DATA;
    }
  }
  if (request.commands_list & DEBUG_COMM) {
    printDebug(F("parsRequest: DEBUG_COMM: parsSMS 2\n"));

    struct ParsRequest request = {0};
    if (parsSMS(2, request, sim800) == SUCCESS) {
      printRequest(request, Serial);
      return_codes |= DEBUG_COMM;
    }
  }

  if (return_codes == request.commands_list) {
    return SUCCESS;
  }
  if (return_codes != EMPTY_COMMAND) {
    return NOT_ABSOLUTE_SUCCESS;
  }
  return ERROR;
}

return_code_t doRequestAsSIM800(struct ParsRequest& request) {
  long return_codes = 0;
  if (request.commands_list & SET_MAX_TEMPERATURE) {
    
    printDebug(F("parsRequest: setMaxTemperature\n"));
    if (setMaxTemperature(request.max_temperature) == SUCCESS) {
      return_codes |= SET_MAX_TEMPERATURE;
    }
  }
  if (request.commands_list & SET_MAX_HUMIDITY) {
    
    printDebug(F("parsRequest: setMaxHumidity\n"));
    if (setMaxHumidity(request.max_humidity) == SUCCESS) {
      return_codes |= SET_MAX_HUMIDITY;
    }
  }
  if (request.commands_list & SET_MIN_TEMPERATURE) {
    
    printDebug(F("parsRequest: setMinTemperature\n"));
    if (setMinTemperature(request.min_temperature) == SUCCESS) {
      return_codes |= SET_MIN_TEMPERATURE;
    }
  }
  if (request.commands_list & SET_MIN_HUMIDITY) {
    
    printDebug(F("parsRequest: setMinHumidity\n"));
    if (setMinHumidity(request.min_humidity) == SUCCESS) {
      return_codes |= SET_MIN_HUMIDITY;
    }
  }
  if (request.commands_list & PRINT_STORED_DATA) {
    printDebug(F("parsRequest: printStoredDataTo\n"));
    if (printStoredDataTo(Serial) == SUCCESS) {
      return_codes |= PRINT_STORED_DATA;
    }
  }
  if (request.commands_list & PRINT_MEASURED_DATA) {
    printDebug(F("parsRequest: printMeasuredDataToSerial\n"));
    if (printMeasuredDataToSerial() == SUCCESS) {
      return_codes |= PRINT_MEASURED_DATA;
    }
  }

  if (return_codes == request.commands_list) {
    return SUCCESS;
  }
  if (return_codes != EMPTY_COMMAND) {
    return NOT_ABSOLUTE_SUCCESS;
  }
  return ERROR;
}