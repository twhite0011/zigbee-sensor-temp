#pragma once

#include <stdbool.h>
#include "esp_zigbee_core.h"
#include "sensors.h"

#define ZIGBEE_MANUFACTURER_NAME    "DIY"
#define ZIGBEE_MODEL_ID             "XIAO-SHTC3"

#define EP_SENSOR                   1

#define ATTR_TEMP_MEASURED_VALUE    ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID
#define ATTR_HUM_MEASURED_VALUE     ESP_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID

void zigbee_register_endpoints(void);
void zigbee_report_sensor_data(const shtc3_data_t *data);
void zigbee_signal_handler(esp_zb_app_signal_t *signal_s);
bool zigbee_is_joined(void);
