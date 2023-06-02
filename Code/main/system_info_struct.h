#pragma once

typedef enum {
  NORMAL = 0,
  HAPPEN,
  SEND,
  WAIT_NORMALISATION
} system_warning_status_t;

struct system_info {
  system_warning_status_t humidity_warning;
  system_warning_status_t temperature_warning;
  system_warning_status_t weight_warning;
  system_warning_status_t power_warning;

  return_code_t sim800_result;
  return_code_t serial_result;

  bool send_measured_data;
  struct date_time send_measured_data_time;

  bool set_send_time;
};
