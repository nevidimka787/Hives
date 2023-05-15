#pragma once

#include <SoftwareSerial.h>

#ifdef SERIAL_DEBUG
#define printError(msg) { Serial.print("ERROR: "); Serial.print(msg); }
#define printDebug(msg) { Serial.print("DEBUG: "); Serial.print(msg); }
#define printDebugInLine(msg); { Serial.print(msg); }
#else // SERIAL_DEBUG
#define printError(msg) {}
#define printDebug(msg) {}
#define printDebugInLine(msg); {}
#endif // SERIAL_DEBUG

