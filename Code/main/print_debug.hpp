#pragma once

#include <SoftwareSerial.h>

#ifdef SERIAL_DEBUG_FULL
#define SERIAL_ERROR
#define SERIAL_WARNING
#define SERIAL_DEBUG
#endif // SERIAL_DEBUG_FULL

#ifdef SERIAL_ERROR
#define printError(msg) { Serial.print(F("ERROR: ")); Serial.print(msg); }
#else // SERIAL_ERROR
#define printError(msg) {}
#endif // SERIAL_ERROR

#ifdef SERIAL_WARNING
#define printWarning(msg) { Serial.print(F("WARNING: ")); Serial.print(msg); }
#else // SERIAL_WARNING
#define printWarning(msg) {}
#endif // SERIAL_WARNING

#ifdef SERIAL_DEBUG
#define printDebug(msg) { Serial.print(F("DEBUG: ")); Serial.print(msg); }
#else // SERIAL_DEBUG
#define printDebug(msg) {}
#endif // SERIAL_DEBUG

#if defined(SERIAL_ERROR) || defined(SERIAL_WARNING) || defined(SERIAL_DEBUG)
#define printDebugInLine(msg); { Serial.print(msg); }
#else // SERIAL_ERROR SERIAL_WARNING SERIAL_DEBUG
#define printDebugInLine(msg); {}
#endif // SERIAL_ERROR SERIAL_WARNING SERIAL_DEBUG
