#pragma once

// Activa o desactiva mensajes de depuración
#ifndef DEBUG_MODE
  #define DEBUG_MODE true
#endif

// TinyGSM también define DBG, así que lo eliminamos
#ifdef DBG
  #undef DBG
#endif

// Declarar función de log de pantalla sin incluir toda la cabecera
extern void displayLog(const String& msg);

// Nuestro macro de depuración
#define DBG(fmt, ...) \
  do { \
    if (DEBUG_MODE) { \
      char _dbg_buf[128]; \
      snprintf(_dbg_buf, sizeof(_dbg_buf), "[APP] " fmt, ##__VA_ARGS__); \
      SerialMon.println(_dbg_buf); \
      displayLog(String(_dbg_buf)); \
    } \
  } while (0)
