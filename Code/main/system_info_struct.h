#pragma once

typedef enum {
  NORMAL = 0,
  SEND,
  WAIT_NORMALISATION
} system_warning_status_t;

typedef enum {
  TIME_OF_SEND_IS_CORRECT = (uint8_t)1 << (uint8_t)0,
  GLOBAL_TIME_IS_SETTED   = (uint8_t)1 << (uint8_t)1
} send_measured_data_flags_t;

struct system_info {
  system_warning_status_t humidity_warning;
  system_warning_status_t temperature_warning;
  system_warning_status_t weight_warning;
  system_warning_status_t power_warning;

  return_code_t sim800_result;
  return_code_t serial_result;

  send_measured_data_flags_t send_measured_data_flags;
  struct date_time send_measured_data_time;
};
