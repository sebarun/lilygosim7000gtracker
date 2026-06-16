#pragma once

// Activa o desactiva mensajes de depuración
#ifndef DEBUG_MODE
  #define DEBUG_MODE true
#endif

// TinyGSM también define DBG, así que lo eliminamos
#ifdef DBG
  #undef DBG
#endif

// Nuestro macro de depuración
#define DBG(fmt, ...) \
  do { if (DEBUG_MODE) SerialMon.printf("[APP] " fmt "\n", ##__VA_ARGS__); } while (0)
