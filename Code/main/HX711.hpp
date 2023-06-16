#pragma once

//
//     FILE: HX711.hpp
//   AUTHOR: Rob Tillaart
// REDACTOR: Nevidimka787
//  VERSION: 0.3.6.1
//  PURPOSE: Library for load cells for Arduino
//      URL: https://github.com/RobTillaart/HX711
//
//  NOTES
//  Superset of interface of HX711 class of Bogde
//  float instead of long as float has 23 bits mantissa.


#include "Arduino.h"
#include "return_codes.hpp"
#include "print_debug.hpp"

#define HX711_LIB_VERSION               (F("0.3.6.1"))


const uint8_t HX711_AVERAGE_MODE = 0x00;
//  in median mode only between 3 and 15 samples are allowed.
const uint8_t HX711_AVERAGE_SQRT_MODE  = 0x01;
//  causes read() to be called only once!
const uint8_t HX711_RAW_MODE     = 0x04;


//  supported values for set_gain()
const uint8_t HX711_CHANNEL_A_GAIN_128 = 128;  // default
const uint8_t HX711_CHANNEL_A_GAIN_64 = 64;
const uint8_t HX711_CHANNEL_B_GAIN_32 = 32;


class HX711 {
public:
  HX711();
  ~HX711();

  //  fixed gain 128 for now
  void     begin(uint8_t dataPin, uint8_t clockPin);

  void     reset();

  //  checks if load cell is ready to read.
  bool     is_ready();

  //  max timeout
  bool     wait_ready_timeout(uint32_t timeout = 1000, uint32_t ms = 0);

  //  raw read
  return_codes::return_code_t HX711::read(float& return_value, unsigned long timeout_ms = 1000);

  //  get average of multiple raw reads
  //  times = 1 or more
  return_codes::return_code_t read_average(float& return_value, uint16_t times = 10, unsigned long timeout_ms = 1000, uint8_t attemps_per_time = 2);
  
  return_codes::return_code_t read_average_square(float& return_value, uint16_t times = 10, unsigned long timeout_ms = 1000, uint8_t attemps_per_time = 2);


  //  get set mode for get_value() and indirect get_units().
  //  in median and medavg mode only 3..15 samples are allowed.
  void     set_raw_mode()           { _mode = HX711_RAW_MODE; };
  void     set_average_mode()       { _mode = HX711_AVERAGE_MODE; };
  void     set_average_sqrt_mode()  { _mode = HX711_AVERAGE_SQRT_MODE; };
  uint8_t  get_mode()               { return _mode; };

  return_codes::return_code_t HX711::read_by_mode(float& return_value, uint8_t times, unsigned long timeout_ms, uint8_t attemps_per_time);
  //  corrected for offset.
  //  in HX711_RAW_MODE the parameter times will be ignored.
  return_codes::return_code_t get_value(float& return_value, uint8_t times = 1, unsigned long timeout_ms = 1000, uint8_t attemps_per_time = 2);
  //  converted to proper units.
  //  in HX711_RAW_MODE the parameter times will be ignored.
  return_codes::return_code_t get_units(float& return_value, uint8_t times = 1, unsigned long timeout_ms = 1000, uint8_t attemps_per_time = 2);
  //  converted to proper units.
  //  in HX711_RAW_MODE the parameter times will be ignored.
  //  if read error couse? then return nan.
  float get_units(uint8_t times = 1, unsigned long timeout_ms = 1000, uint8_t attemps_per_time = 2);


  //  TARE
  //  call tare to calibrate zero
  return_codes::return_code_t tare(uint8_t times = 10, unsigned long timeout_ms = 1000, uint8_t attemps_per_time = 2) {
    return_codes::return_code_t result = read_by_mode(_offset, times, timeout_ms, attemps_per_time);
    
    printDebug(F("HX711::tare: _offset: "));
    printDebugInLine(_offset);
    printDebugInLine('\n');

    return result;
  };
  float    get_tare()                   { return -_offset * _scale; };
  bool     tare_set()                   { return _offset != 0.0f; };


  //  CORE "CONSTANTS" -> read datasheet
  //  CHANNEL      GAIN   notes
  //  -------------------------------------
  //     A         128    default, tested
  //     A          64
  //     B          32

  //  returns true  ==>  parameter gain is valid
  //  returns false ==>  parameter gain is invalid ==> no change.
  //  note that changing gain/channel takes up to 400 ms (page 3)
  //  if forced == true, the gain will be forced set 
  //  even it is already the right value
  bool     set_gain(uint8_t gain = HX711_CHANNEL_A_GAIN_128, bool forced = false, unsigned long timeout_ms = 1000);
  uint8_t  get_gain();


  //  SCALE > 0
  void     set_scale(float scale = 1.0) { _scale = 1.0 / scale; };
  float    get_scale()                  { return 1.0 / _scale; };

  //  OFFSET > 0
  void     set_offset(float offset = 0) { _offset = offset; };
  float    get_offset()                 { return _offset; };


  //  CALIBRATION
  //  clear the scale
  //  call tare() to set the zero offset
  //  put a known weight on the scale
  //  call calibrate_scale(weight)
  //  scale is calculated.
  return_codes::return_code_t calibrate_scale(float weight, uint8_t times = 10, unsigned long timeout_ms = 1000, uint8_t attemps_per_time = 2);


  //  POWER MANAGEMENT
  void     power_down();
  void     power_up();

private:
  uint8_t  _dataPin;
  uint8_t  _clockPin;

  uint8_t  _gain     = 128;     // default channel A
  float    _offset   = 0.0f;
  float    _scale    = 1.0f;
  uint8_t  _mode     = 0;

  void     _insertSort(float * array, uint8_t size);
  uint8_t  _shiftIn();
};

HX711::HX711() {
  reset();
}


HX711::~HX711() {}

void HX711::begin(uint8_t dataPin, uint8_t clockPin) {
  _dataPin  = dataPin;
  _clockPin = clockPin;

  pinMode(_dataPin, INPUT);
  pinMode(_clockPin, OUTPUT);
  digitalWrite(_clockPin, LOW);

  reset();
}


void HX711::reset() {
  power_down();
  power_up();
  _offset   = 0.0f;
  _scale    = 1;
  _gain     = HX711_CHANNEL_A_GAIN_128;
  _mode     = HX711_AVERAGE_MODE;
}


bool HX711::is_ready() {
  return digitalRead(_dataPin) == LOW;
}


//  From datasheet page 4
//  When output data is not ready for retrieval,
//       digital output pin DOUT is HIGH.
//  Serial clock input PD_SCK should be LOW.
//  When DOUT goes to LOW, it indicates data is ready for retrieval.
return_codes::return_code_t HX711::read(float& return_value, unsigned long timeout_ms) {
  //  this BLOCKING wait takes most time...
  unsigned long start_millis = millis();
  while (digitalRead(_dataPin) == HIGH) {
    if (millis() - start_millis > timeout_ms) {
      return return_codes::ERROR;
    }
    yield();
  } 

  union {
    long value = 0;
    uint8_t data[4];
  } v;

  //  blocking part ...
  noInterrupts();

  //  Pulse the clock pin 24 times to read the data.
  //  v.data[2] = shiftIn(_dataPin, _clockPin, MSBFIRST);
  //  v.data[1] = shiftIn(_dataPin, _clockPin, MSBFIRST);
  //  v.data[0] = shiftIn(_dataPin, _clockPin, MSBFIRST);
  v.data[2] = _shiftIn();
  v.data[1] = _shiftIn();
  v.data[0] = _shiftIn();

  //  TABLE 3 page 4 datasheet
  //
  //  CLOCK      CHANNEL      GAIN      m
  //  ------------------------------------
  //   25           A         128       1    //  default
  //   26           B          32       2
  //   27           A          64       3
  //
  //  only default 128 verified,
  //  selection goes through the set_gain(gain)
  //
  uint8_t m = 1;
  if      (_gain == HX711_CHANNEL_A_GAIN_128) m = 1;
  else if (_gain == HX711_CHANNEL_A_GAIN_64)  m = 3;
  else if (_gain == HX711_CHANNEL_B_GAIN_32)  m = 2;

  while (m > 0) {
    //  delayMicroSeconds(1) needed for fast processors?
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_clockPin, LOW);
    m--;
  }

  interrupts();
  //  yield();

  //  SIGN extend
  if (v.data[2] & 0x80) v.data[3] = 0xFF;

  return_value = (float)v.value;

  return return_codes::SUCCESS;
}


//  note: if parameter gain == 0xFF40 some compilers
//  will map that to 0x40 == HX711_CHANNEL_A_GAIN_64;
//  solution: use uint32_t or larger parameters everywhere.
//  note that changing gain/channel may take up to 400 ms (page 3)
bool HX711::set_gain(uint8_t gain, bool forced, unsigned long timeout_ms) {
  if ( (not forced) && (_gain == gain)) return true;
  switch(gain) {
    case HX711_CHANNEL_B_GAIN_32:
    case HX711_CHANNEL_A_GAIN_64:
    case HX711_CHANNEL_A_GAIN_128:
      _gain = gain;
      float temp;
      return read(temp, timeout_ms) == return_codes::SUCCESS ? true : false;     //  next user read() is from right channel / gain
  }
  return false;   //  unchanged, but incorrect value.
}


uint8_t HX711::get_gain() {
  return _gain;
}


//  assumes tare() has been set.
return_codes::return_code_t HX711::calibrate_scale(float weight, uint8_t times, unsigned long timeout_ms, uint8_t attemps_per_time) {
  float value = 0.0f;
  if (get_value(value, times, timeout_ms, attemps_per_time) != return_codes::SUCCESS) {
    return return_codes::ERROR;
  }
  _scale = weight / value;
  printDebug(F("HX711::calibrate_scale: weight: "));
  printDebugInLine(weight);
  printDebugInLine(F(" value: "));
  printDebugInLine(value);
  printDebugInLine('\n');

  return return_codes::SUCCESS;
}

bool HX711::wait_ready_timeout(uint32_t timeout, uint32_t ms) {
  uint32_t start = millis();
  while (millis() - start < timeout) {
    if (is_ready()) return true;
    delay(ms);
  }
  return false;
}

return_codes::return_code_t HX711::read_average(float& return_value, uint16_t times, unsigned long timeout_ms, uint8_t attemps_per_time) {
  if (times < 1) times = 1;
  return_value = 0.0f;
  float temp = 0.0f;
  uint8_t attemps = 0;
  for (uint8_t i = 0; i < times; i++) {
    attemps = 0;
    while (read(temp, timeout_ms) != return_codes::SUCCESS) {
      ++attemps;
      if (attemps >= attemps_per_time) {
        printError(F("HX711::read_average: max attemps reach, time: "));
        printDebugInLine(i + 1);
        printDebugInLine(F(" / "));
        printDebugInLine(times);
        printDebugInLine('\n');
        return return_codes::ERROR;
      }
    }
    return_value += temp;
    yield();
  }
  return_value /= (float)times;
  return return_codes::SUCCESS;
}

return_codes::return_code_t HX711::read_average_square(float& return_value, uint16_t times, unsigned long timeout_ms, uint8_t attemps_per_time) {
  if (times < 1) times = 1;
  return_value = 0.0f;
  float temp = 0.0f;
  uint8_t attemps = 0;
  for (uint8_t i = 0; i < times; i++) {
    attemps = 0;
    while (read(temp, timeout_ms) != return_codes::SUCCESS) {
      ++attemps;
      if (attemps >= attemps_per_time) {
        return return_codes::ERROR;
      }
    }
    if (temp > 0.0f) {
      return_value += temp * temp;
    } else {
      return_value -= temp * temp;
    }
    yield();
  }

  if (return_value > 0) {
    return_value = sqrt(return_value / (float)times);
    return return_codes::SUCCESS;
  }
  return_value = -sqrt(-return_value / (float)times);
  return return_codes::SUCCESS;
}

return_codes::return_code_t HX711::read_by_mode(float& return_value, uint8_t times, unsigned long timeout_ms, uint8_t attemps_per_time) {
  switch(_mode) {
  case HX711_RAW_MODE:
    return read(return_value, timeout_ms);
  case HX711_AVERAGE_SQRT_MODE:
    return read_average_square(return_value, times, timeout_ms, attemps_per_time);
  case HX711_AVERAGE_MODE:
    return read_average(return_value, times, timeout_ms, attemps_per_time);
  default:
    return return_codes::ERROR;
  }
}

return_codes::return_code_t HX711::get_value(float& return_value, uint8_t times, unsigned long timeout_ms, uint8_t attemps_per_time) {
  if (read_by_mode (return_value, times, timeout_ms, attemps_per_time) != return_codes::SUCCESS) {
    return return_codes::ERROR;
  }
  return_value -= _offset;
  return return_codes::SUCCESS;
};

return_codes::return_code_t HX711::get_units(float& return_value, uint8_t times, unsigned long timeout_ms, uint8_t attemps_per_time) {
  if (get_value(return_value, times, timeout_ms, attemps_per_time) != return_codes::SUCCESS) {
    return return_codes::ERROR;
  }
  return_value *= _scale;
  return return_codes::SUCCESS;
};

float HX711::get_units(uint8_t times, unsigned long timeout_ms, uint8_t attemps_per_time) {
  // power_up();
  float return_value = 0.0f;
  if (get_units(return_value, times, timeout_ms, attemps_per_time) != return_codes::SUCCESS) {
    return 0.0f / 0.0f;
  }
  // power_down();
  return return_value;
};


void HX711::power_down() {
  //  at least 60 us HIGH
  digitalWrite(_clockPin, HIGH);
  delayMicroseconds(64);
}


void HX711::power_up() {
  digitalWrite(_clockPin, LOW);
}


//  MSB_FIRST optimized shiftIn
//  see datasheet page 5 for timing
uint8_t HX711::_shiftIn() {
  //  local variables are faster.
  uint8_t clk   = _clockPin;
  uint8_t data  = _dataPin;
  uint8_t value = 0;
  uint8_t mask  = 0x80;
  while (mask > 0) {
    digitalWrite(clk, HIGH);
    delayMicroseconds(1);   //  T2  >= 0.2 us
    if (digitalRead(data) == HIGH)
    {
      value |= mask;
    }
    digitalWrite(clk, LOW);
    delayMicroseconds(1);   //  keep duty cycle ~50%
    mask >>= 1;
  }
  return value;
}


//  -- END OF FILE --
