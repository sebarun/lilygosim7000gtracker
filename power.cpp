#include "power.h"
#include "logger.h"
#include <esp_sleep.h>
void powerInit(){}
void enterDeepSleep(){logMsg("Sleep");esp_sleep_enable_timer_wakeup(5000000);esp_deep_sleep_start();}