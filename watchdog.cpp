#include "watchdog.h"
#include "config.h"
#include <esp_task_wdt.h>
#include <esp_idf_version.h>
#include <rom/rtc.h>

void Watchdog::init() {
#if ESP_IDF_VERSION_MAJOR >= 5
    esp_task_wdt_config_t twdt_config = {
        .timeout_ms    = (uint32_t)WATCHDOG_TIMEOUT_S * 1000,
        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
        .trigger_panic  = true
    };
    esp_task_wdt_init(&twdt_config);
#else
    esp_task_wdt_init(WATCHDOG_TIMEOUT_S, true);
#endif
    esp_task_wdt_add(NULL);
}

void Watchdog::feed() {
    esp_task_wdt_reset();
}

const char* Watchdog::bootReasonStr() {
    switch (esp_reset_reason()) {
        case ESP_RST_POWERON:   return "POWER_ON";
        case ESP_RST_EXT:       return "EXTERNAL_RESET";
        case ESP_RST_SW:        return "SOFTWARE_RESET";
        case ESP_RST_PANIC:     return "PANIC";
        case ESP_RST_INT_WDT:   return "INT_WATCHDOG";
        case ESP_RST_TASK_WDT:  return "TASK_WATCHDOG";
        case ESP_RST_WDT:       return "OTHER_WATCHDOG";
        case ESP_RST_DEEPSLEEP: return "DEEP_SLEEP";
        case ESP_RST_BROWNOUT:  return "BROWNOUT";
        default:                return "UNKNOWN";
    }
}
