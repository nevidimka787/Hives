#pragma once

#include <Stream.h>

#include "eeprom_commands.hpp"
#include "return_codes.hpp"
#include "send_commands.hpp"

using namespace return_codes;

// get request
// execute all commands
// @param request - input request
// @param global_system_info - system information (can be changed)
// return SUCCES if all commands was executed successfully or was no commands in request, NOT_ABSOLUTE_SUCCESS if some commands was exacuted successfully, ERROR if nothing was executed SUCCESSFULLY
return_code_t doRequestAsSerial(const struct ParsRequest& request, struct system_info& global_system_info);

// get request
// execute commands that available for users
// skip other commands
// @param request - input request
// @param global_system_info - system information (can be changed)
// return SUCCES if all commands for users was executed successfully or was no commands for users in request, NOT_ABSOLUTE_SUCCESS if some commands for users was exacuted successfully, ERROR if nothing was executed SUCCESSFULLY
return_code_t doRequestAsSIM800(const struct ParsRequest& request, struct system_info& global_system_info);

// sen default hardcoded SMS to seted phone number
return_code_t sendDefaultSMS(Stream& to_serial = sim800);

return_code_t doRequestAsSerial(const struct ParsRequest& request, struct system_info& global_system_info) {
  long return_codes = 0;
  if (request.commands_list & DELETE_SMS_ALL) {
    printDebug(F("parsRequest: deleteSMSAll\n"));
    if (deleteSMSAll() == SUCCESS) {
      return_codes |= DELETE_SMS_ALL;
    }
  }
  if (request.commands_list & CALCULATE_SCALE) {
    printDebug(F("parsRequest: calculateScale\n"));
    if(calculateScale(request.target_weight) == SUCCESS) {
      return_codes |= CALCULATE_SCALE;
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
    if (Sim800Config() == SUCCESS) {
      return_codes |= CMGF_EN;
    }
  }
  if (request.commands_list & GET_TIME) {
    printDebug(F("parsRequest: getCurrentTimeInSeconds\n"));

    unsigned long time_in_sec;
    if (getCurrentTimeInSeconds(time_in_sec) == SUCCESS) {
      return_codes |= GET_TIME;
    }
    printDebug(F("parsRequest: Time: "));
    printDebugInLine(time_in_sec);
    printDebugInLine('\n');
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
  if (request.commands_list & SET_MAX_WEIGHT) {
    printDebug(F("parsRequest: setMaxWeight\n"));
    if (setMaxWeight(request.max_weight) == SUCCESS) {
      return_codes |= SET_MAX_WEIGHT;
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
  if (request.commands_list & SET_MIN_WEIGHT) {
    printDebug(F("parsRequest: setMinxWeight\n"));
    if (setMinWeight(request.min_weight) == SUCCESS) {
      return_codes |= SET_MIN_WEIGHT;
    }
  }
  if (request.commands_list & SET_WEIGHT_OFFSET) {
    
    printDebug(F("parsRequest: setScalesOffset\n"));
    if (setScalesOffset(request.weight_offset) == SUCCESS) {
      return_codes |= SET_WEIGHT_OFFSET;
    }
  }
  if (request.commands_list & SET_WEIGHT_SCALE) {
    
    printDebug(F("parsRequest: setScalesScale\n"));
    if (setScalesScale(request.weight_scale) == SUCCESS) {
      return_codes |= SET_WEIGHT_SCALE;
    }
  }
  if (request.commands_list & TARE) {
    printDebug(F("parsRequest: tareScales\n"));
    if(tareScales() == SUCCESS) {
      return_codes |= TARE;
    }
  }
  if (request.commands_list & SET_SEND_TIME) {
    printDebug(F("parsRequest: setSendTime\n"));
    bool in_limits = false;
    if (setSendTime(request.date_time, in_limits) == SUCCESS) {
      if (in_limits) {
        global_system_info.send_measured_data_flags |= TIME_OF_SEND_IS_CORRECT;
      } else {
        global_system_info.send_measured_data_flags &= ~TIME_OF_SEND_IS_CORRECT;
      }
      return_codes |= SET_SEND_TIME;
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
    if (printMeasuredDataTo(Serial) == SUCCESS) {
      return_codes |= PRINT_MEASURED_DATA;
    }
  }
  if (request.commands_list & UPDATE_DATE_TIME) {
    printDebug(F("parsRequest: updateDateTime\n"));
    struct date_time date_time_i;
    if (updateDateTime(date_time_i, sim800) == SUCCESS) {
      return_codes |= UPDATE_DATE_TIME;
    }

  }
  if (request.commands_list & DEBUG_COMM) {
    printDebug(F("parsRequest: DEBUG_COMM: sendDefaultSMS\n"));

    return_code_t result = sendDefaultSMS();
    if (result == SUCCESS) {
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

return_code_t doRequestAsSIM800(const struct ParsRequest& request, struct system_info& global_system_info) {
  long return_codes = 0;

  if (request.commands_list & (PRINT_STORED_DATA | PRINT_MEASURED_DATA)) {
    char phone_number[20];
    getPhoneNumber(phone_number);
    sim800.print(F("AT+CMGS=\"+"));
    sim800.print(phone_number);
    sim800.println(F("\""));
    waitAvailable(sim800);
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
  if (request.commands_list & SET_MAX_WEIGHT) {
    
    printDebug(F("parsRequest: setMaxWeight\n"));
    if (setMaxWeight(request.max_weight) == SUCCESS) {
      return_codes |= SET_MAX_WEIGHT;
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
  if (request.commands_list & SET_MIN_WEIGHT) {
    
    printDebug(F("parsRequest: setMinWeight\n"));
    if (setMinWeight(request.min_weight) == SUCCESS) {
      return_codes |= SET_MIN_WEIGHT;
    }
  }
  if (request.commands_list & SET_WEIGHT_OFFSET) {
    
    printDebug(F("parsRequest: setScalesOffset\n"));
    if (setScalesOffset(request.weight_offset) == SUCCESS) {
      return_codes |= SET_WEIGHT_OFFSET;
    }
  }
  if (request.commands_list & SET_WEIGHT_SCALE) {
    
    printDebug(F("parsRequest: setScalesScale\n"));
    if (setScalesScale(request.weight_scale) == SUCCESS) {
      return_codes |= SET_WEIGHT_SCALE;
    }
  }
  if (request.commands_list & CALCULATE_SCALE) {
    printDebug(F("parsRequest: calculateScale\n"));
    if(calculateScale(request.target_weight) == SUCCESS) {
      return_codes |= CALCULATE_SCALE;
    }
  }
  if (request.commands_list & TARE) {
    printDebug(F("parsRequest: tareScales\n"));
    if(tareScales() == SUCCESS) {
      return_codes |= TARE;
    }
  }
  if (request.commands_list & SET_SEND_TIME) {
    printDebug(F("parsRequest: setSendTime\n"));
    bool in_limits = false;
    if (setSendTime(request.date_time, in_limits) == SUCCESS) {
      if (in_limits) {
        global_system_info.send_measured_data_flags |= TIME_OF_SEND_IS_CORRECT;
      } else {
        global_system_info.send_measured_data_flags &= ~TIME_OF_SEND_IS_CORRECT;
      }
      return_codes |= SET_SEND_TIME;
    }
  }
  if (request.commands_list & PRINT_STORED_DATA) {
    printDebug(F("parsRequest: printStoredDataTo\n"));
    if (shortPrintStoredDataTo(sim800) == SUCCESS) {
      return_codes |= PRINT_STORED_DATA;
    }
  }
  if (request.commands_list & PRINT_MEASURED_DATA) {
    printDebug(F("parsRequest: printMeasuredDataToSerial\n"));
    if (shortPrintMeasuredDataTo(sim800) == SUCCESS) {
      return_codes |= PRINT_MEASURED_DATA;
    }
  }

  if (request.commands_list & (PRINT_STORED_DATA | PRINT_MEASURED_DATA)) {
    if (return_codes & (PRINT_STORED_DATA | PRINT_MEASURED_DATA)) {
      sim800.println((char)26); // send message
      
      return_codes &= ~(PRINT_STORED_DATA | PRINT_MEASURED_DATA);
      return_codes |=
      checkSim800OK(10000) == SUCCESS ?
      request.commands_list & (PRINT_STORED_DATA | PRINT_MEASURED_DATA) :
      0;
    } else {
      sim800.println((char)27); // cancel
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

return_code_t sendDefaultSMS(Stream& to_serial = sim800) {
  char phone_number[20];
  getPhoneNumber(phone_number);
  to_serial.print(F("AT+CMGS=\"+"));
  to_serial.print(phone_number);
  to_serial.println(F("\""));
  
  waitAvailable(to_serial);
  to_serial.println(F("Default message from SIM800.\n"));
  to_serial.write((char)26);

  for(char attemp = 0; attemp < 10; ++attemp) {
    if (checkSim800OK(1000) == SUCCESS) {
      return SUCCESS;
    }
  }
  return ERROR;
}
