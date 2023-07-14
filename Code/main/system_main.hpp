#pragma once

#include "blink.hpp"
#include "eeprom_commands.hpp"
#include "events_manager.hpp"
#include "message_generator.hpp"
#include "return_codes.hpp"
#include "system_info_struct.h"

using namespace return_codes;

// if value grater then max or less then min, then change status.
// @param value - input value
// @param max - high edge
// @param min - low edge
// @param current_status - status of this value (can be changed)
void checkMaxMin(float value, float max, float min, system_warning_status_t& current_status);

// get last SMS from sim800
// execute commands from SMS as user
// @param result_system_info - system information
// @return SUCCESS if some comands was executed, NO_REQUEST if nothing was executed, ERROR if parsing or exeqution failure
return_code_t systemDoSMS(struct system_info& result_system_info);

// try to fix system errors
// reconfigure sim800
// @return SUCCESS if fix was successfully
return_code_t systemFixAction();

// execute SMS if available
// update send time if available
// @param global_system_info - system information
// @return ERROR if SMS execution or time update failure, else SUCCESS
return_code_t systemMainAction(struct system_info& global_system_info);

// get time of send SMS from eeprom
// @param result_system_info - system information
// @return always SUCCESS
return_code_t systemSetSendTime(struct system_info& result_system_info);

// skip past events
// @param global_system_info - system information
// @return SUCCESS if global time and event time are valid, else ERROR
return_code_t systemSkipPastEvents(struct system_info& global_system_info);

// try to update global time in system
// @param result_system_info - system information
// @return always SUCCESS
return_code_t systemUpdateGlobalTime(struct system_info& result_system_info, unsigned long& date_time_last_update_time_point, unsigned long& date_time_update_time_point);

// update time points for all events
// @param result_system_info - system information
// return always SUCCESS
return_code_t systemUpdateSendTimePoint(struct system_info& result_system_info);

void checkMaxMin(float value, float max, float min, system_warning_status_t& current_status) {
  if (value > max || value < min) { // if max or min is NAN then egde isn't checked
    if (current_status == WAIT_NORMALISATION) {
      return;
    }
    current_status = SEND;
    return;
  }
   
  if (current_status == WAIT_NORMALISATION) {
    printDebug(F("checkMaxMin: value: "));
    printDebugInLine(value);
    printDebugInLine(F(" in range: ( "));
    printDebugInLine(max);
    printDebugInLine(F(" : "));
    printDebugInLine(min);
    printDebugInLine(F(" ) return to normal\n"));

    current_status = NORMAL;
  }
  return;
}

return_code_t systemMainAction(struct system_info& global_system_info) {
  LEDOn();

  return_code_t result = SUCCESS;
  
  result = systemDoSMS(global_system_info);
  if (result == ERROR) {
    return ERROR;
  }

  if (global_system_info.send_measured_data_flags & TIME_OF_SEND_MUST_BE_UPDATED) {
    result = systemSetSendTime(global_system_info);
    result = systemUpdateSendTimePoint(global_system_info);
    result = systemSkipPastEvents(global_system_info);

    global_system_info.send_measured_data_flags &= ~TIME_OF_SEND_MUST_BE_UPDATED; // clear flag after update
  }

  struct StoredData stored_data = getStoredData();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  scale.set_offset(stored_data.weight_offset);
  scale.set_scale(stored_data.weight_scale);
  float weight = scale.get_units(10, 1000);

  checkMaxMin(humidity, stored_data.max_humidity, stored_data.min_humidity, global_system_info.humidity_warning);
  checkMaxMin(temperature, stored_data.max_temperature, stored_data.min_temperature, global_system_info.temperature_warning);
  checkMaxMin(weight, stored_data.max_weight, stored_data.min_weight, global_system_info.weight_warning);
  
  if (global_system_info.humidity_warning == SEND || global_system_info.temperature_warning == SEND || global_system_info.weight_warning == SEND) {
    if (global_system_info.humidity_warning == SEND) {
      global_system_info.humidity_warning = WAIT_NORMALISATION;
    }
    if (global_system_info.temperature_warning == SEND) {
      global_system_info.temperature_warning = WAIT_NORMALISATION;
    }
    if (global_system_info.weight_warning == SEND) {
      global_system_info.weight_warning = WAIT_NORMALISATION;
    }
#ifdef SERIAL_WARNING
    if (global_system_info.humidity_warning == WAIT_NORMALISATION) {
      Serial.print(F("WARNING: Humidity out of range!\n"));
    }
    if (global_system_info.temperature_warning == WAIT_NORMALISATION) {
      Serial.print(F("WARNING: Temperature out of range!\n"));
    }
    if (global_system_info.weight_warning == WAIT_NORMALISATION) {
      Serial.print(F("WARNING: Weight out of range!\n"));
    }
    
    shortPrintMeasuredDataTo(Serial);
    shortPrintStoredDataTo(Serial);
#endif // SERIAL_DEBUG

    // return result;

    char phone_number[20];
    getPhoneNumber(phone_number);
    sim800.print(F("AT+CMGS=\"+"));
    sim800.print(phone_number);
    sim800.println(F("\""));
    waitAvailable(sim800);

    if (global_system_info.humidity_warning == WAIT_NORMALISATION) {
      sim800.print(F("WARNING: Humidity out of range!\n"));
    }
    if (global_system_info.temperature_warning == WAIT_NORMALISATION) {
      sim800.print(F("WARNING: Temperature out of range!\n"));
    }
    if (global_system_info.weight_warning == WAIT_NORMALISATION) {
      sim800.print(F("WARNING: Weight out of range!\n"));
    }

    shortPrintMeasuredDataTo(sim800);
    shortPrintStoredDataTo(sim800);

    sim800.println((char)26);

    for(char attemp = 0; attemp < 10; ++attemp) {
      if (checkSim800OK(1000) == SUCCESS) {
        return SUCCESS;
      }
    }
    return ERROR;
  }

  return result;
}

return_code_t systemFixAction() {
  return Sim800Config();
}

return_code_t systemSkipPastEvents(struct system_info& result_system_info) {
  if (result_system_info.send_measured_data_flags & (TIME_OF_SEND_IS_CORRECT | GLOBAL_TIME_IS_SETTED) != (TIME_OF_SEND_IS_CORRECT | GLOBAL_TIME_IS_SETTED)) {
    return ERROR;
  }

  unsigned while_limit = 10000;

  while (eventAvailable(result_system_info.send_measured_data_time) == SUCCESS && while_limit > 0) {
#ifdef SERIAL_DEBUG
    printDebug(F("systemSkipPastEvents: skip: "));
    printDateTime(result_system_info.send_measured_data_time, Serial);
    printDebug('\n');
#endif // SERIAL_DEBUG

    if (setNextDay(result_system_info.send_measured_data_time) != SUCCESS) {
      printError(F("systemSkipPastEvents: invalid next day\n"));
      return ERROR;
    }

    --while_limit;
  }

  return while_limit > 0 ? SUCCESS : ERROR;
}

return_code_t systemUpdateGlobalTime(struct system_info& result_system_info, unsigned long& date_time_last_update_time_point, unsigned long& date_time_update_time_point) {
  result_system_info.sim800_result = updateDateTime(global_date_time, sim800);

  if (result_system_info.sim800_result == SUCCESS) {
    // update at 0:00 am
    date_time_last_update_time_point = millis(); // must be for getCurrentTimeInSeconds
    date_time_update_time_point = date_time_last_update_time_point + getSecondsToNextDay(global_date_time) * 1000LU;
    
    result_system_info.send_measured_data_flags |=  GLOBAL_TIME_IS_SETTED;
  } else {
    // try update after one minute
    date_time_update_time_point = millis() + 60000LU;
    result_system_info.send_measured_data_flags &=  ~GLOBAL_TIME_IS_SETTED;
  }
  
#ifdef SERIAL_DEBUG
  if (result_system_info.sim800_result != SUCCESS) {
    printError(F("systemUpdateGlobalTime: invalid date\n"));
  }
#endif // SERIAL_DEBUG

  return result_system_info.sim800_result;
}

return_code_t systemUpdateSendTimePoint(struct system_info& result_system_info) {
  result_system_info.send_measured_data_time.day = global_date_time.day;
  result_system_info.send_measured_data_time.month = global_date_time.month;
  result_system_info.send_measured_data_time.year = global_date_time.year;
}

return_code_t systemSetSendTime(struct system_info& result_system_info) {
  getSendTime(result_system_info.send_measured_data_time);
  if (checkTimeValid(result_system_info.send_measured_data_time) == SUCCESS) {
    result_system_info.send_measured_data_flags |= TIME_OF_SEND_IS_CORRECT;
  } else {
    result_system_info.send_measured_data_flags &= ~TIME_OF_SEND_IS_CORRECT;
  }

#ifdef SERIAL_DEBUG
  printDebug("systemGetSendTime: time of send is ");
  if (result_system_info.send_measured_data_flags & TIME_OF_SEND_IS_CORRECT) {
    printDebugInLine(F("correct\n"));
  } else {
    printDebugInLine(F("incorrect\n"));
  }
#endif // SERIAL_DEBUG

  return SUCCESS;
}

return_code_t systemDoSMS(struct system_info& result_system_info) {
  int last_sms_id = 0;
  return_code_t result = SUCCESS;
  
  result = getLastSMSId(last_sms_id);
  if (last_sms_id == 0) {
    return NO_REQUEST;
  }
  
  struct ParsRequest request = {0};

  if (parsSMS(last_sms_id, request) != SUCCESS) {
    printError(F("system_main_action: parsSMS error\n"));
    return ERROR;
  }

  result = doRequestAsSIM800(request, result_system_info);
  if (result == ERROR) {
    printError(F("system_main_action: doRequestAsSIM800 error\n"));
    return ERROR;
  }

  deleteSMSAll();

  return SUCCESS;
}
