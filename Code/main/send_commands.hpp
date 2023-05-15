#pragma once

#include <SoftwareSerial.h>

#include "print_debug.hpp"
#include "return_codes.hpp"
#include "str_macroses.hpp"

enum {
  O_DETECTED = 1,
  K_DETECTED = 2
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

return_code_t checkSim800OK() {
  int status = 0;

  if (!sim800.available()) {
    printError("checkSim800OK: no response\n");
    return ERROR;
  }

  while (sim800.available()) {
    char symbol = sim800.read();
  
    if (status == K_DETECTED) {
      if (symbol == '\n') {
        return SUCCESS;
      }
      if (!isSpaceSymbol(symbol)) {
        status = 0;
      }
    }
    if (status == O_DETECTED) {
      if (symbol == 'K') {
        status = K_DETECTED;
      } else {
        status = 0;
      }
    } else if (symbol == 'O') {
      status = O_DETECTED;
    }
  }
  
  printError("checkSim800OK: invalid response\n");
  return ERROR;
}

return_code_t Sim800Check(int response_delay = 1000) {
  sim800.println("AT");

  delay(response_delay);
  return checkSim800OK();
}

return_code_t Sim800Config_CMGF(int arg = 1, int response_delay = 100) {
  sim800.print("AT+CMGF=");
  sim800.println(arg);
  delay(response_delay);
  return checkSim800OK();
}

return_code_t Sim800SendMessage(int phone_number, char message[4096], int response_delay = 100) {
  sim800.print("AT+CMGS=\"+");
  sim800.print(phone_number);
  sim800.println("\"");

  delay(response_delay);
  if (!sim800.available() || sim800.read() != '>') {
    printError("Failed to send message.\n");
    return ERROR;
  }

  sim800.print(message);
  sim800.print((char)27); // send message

  return checkSim800OK();
}

return_code_t printSMSToSerial() {
  sim800.println("AT+CMGL=\"ALL\"");
  
  Serial.println("printSMSToSerial:");

  char c;
  int p = 0;

  while(1) {
WHILE_BEGIN:
    if (sim800.available()) {
      c = (char)sim800.read();

      Serial.print(c);
    } else {
      for (int i = 0; i < 100; ++i) {
        delay(10);
        if (sim800.available()) {
          goto WHILE_BEGIN;
        }
      }
      break;
    }
  }

  return SUCCESS;
}
