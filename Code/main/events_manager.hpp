#pragma once

#include "date_time_struct.hpp"

/*
The function use millis.
@param event_time - time then event will be executed
@param update_time - time of the last time update request
@param date_time_update_time_point - value of counter (millis) then time was updated
@return SUCCES if event is available, else return ERROR
*/
return_code_t eventAvailable(struct date_time& event_time, struct date_time& updated_time, unsigned long date_time_update_time_point) {
  unsigned long seconds_from_update_to_event = getSecondsBeetweenDates(event_time, updated_time);
  unsigned long seconds_from_update_to_now = (millis() - date_time_update_time_point) / 1000L;
  return seconds_from_update_to_now > seconds_from_update_to_event ? SUCCESS : ERROR;
}

/*
The function use millis.
@param event_time_point - time in milliseconds in which event will be executed
@param max_time_to_time_point - maximum dellay to event
@return SUCCES if event is available, else return ERROR
*/
return_code_t eventAvailable(unsigned long event_time_point, unsigned long max_time_to_time_point = 3600000UL * 24) {
  return event_time_point - millis() + max_time_to_time_point <= max_time_to_time_point ? SUCCESS : ERROR;
}

return_code_t eventAvailable(struct date_time& event_date_time) {
  if (compareDateTime(event_date_time, global_date_time) < 0) {
    printDebug("eventAvailable: event_date_time < global_date_time\n");
    return SUCCESS;
  }

  unsigned long current_seconds;
  getCurrentTimeInSeconds(current_seconds);
  unsigned long event_seconds = getDaysBeetweenDates(event_date_time, global_date_time) * 3600LU * 24 + getTimeInSecconds(event_date_time);

  return event_seconds <= current_seconds ? SUCCESS : ERROR;
}







