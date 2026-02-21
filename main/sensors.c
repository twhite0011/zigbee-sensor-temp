#include "sensors.h"

#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "sensors";

static i2c_master_bus_handle_t s_i2c_bus = NULL;
static i2c_master_dev_handle_t s_shtc3_dev = NULL;

#define SHTC3_CMD_WAKEUP            0x3517
#define SHTC3_CMD_SLEEP             0xB098
#define SHTC3_CMD_MEAS_T_FIRST_NM   0x7866

static uint8_t crc8_poly31(const uint8_t *data, size_t len)
{
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int b = 0; b < 8; b++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
        }
    }
    return crc;
}

static esp_err_t shtc3_send_cmd(uint16_t cmd)
{
    uint8_t buf[2] = { (uint8_t)(cmd >> 8), (uint8_t)(cmd & 0xFF) };
    return i2c_master_transmit(s_shtc3_dev, buf, 2, 100);
}

static esp_err_t shtc3_wakeup(void)
{
    esp_err_t ret = shtc3_send_cmd(SHTC3_CMD_WAKEUP);
    vTaskDelay(pdMS_TO_TICKS(2));
    return ret;
}

static esp_err_t shtc3_sleep(void)
{
    return shtc3_send_cmd(SHTC3_CMD_SLEEP);
}

esp_err_t sensors_init(void)
{
    esp_err_t ret;

    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_MASTER_PORT,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ret = i2c_new_master_bus(&bus_cfg, &s_i2c_bus);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C bus init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    i2c_device_config_t shtc3_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SHTC3_I2C_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    ret = i2c_master_bus_add_device(s_i2c_bus, &shtc3_cfg, &s_shtc3_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SHTC3 add device failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = shtc3_wakeup();
    if (ret == ESP_OK) {
        (void)shtc3_sleep();
    }

    ESP_LOGI(TAG, "SHTC3 initialized on I2C 0x%02X", SHTC3_I2C_ADDR);
    return ret;
}

esp_err_t sensors_read_shtc3(shtc3_data_t *out)
{
    if (out == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = shtc3_wakeup();
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "SHTC3 wakeup failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = shtc3_send_cmd(SHTC3_CMD_MEAS_T_FIRST_NM);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "SHTC3 measure cmd failed: %s", esp_err_to_name(ret));
        return ret;
    }

    vTaskDelay(pdMS_TO_TICKS(13));

    uint8_t raw[6] = {0};
    ret = i2c_master_receive(s_shtc3_dev, raw, sizeof(raw), 100);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "SHTC3 read failed: %s", esp_err_to_name(ret));
        return ret;
    }

    if (crc8_poly31(&raw[0], 2) != raw[2] || crc8_poly31(&raw[3], 2) != raw[5]) {
        ESP_LOGW(TAG, "SHTC3 CRC mismatch");
        return ESP_ERR_INVALID_CRC;
    }

    uint16_t t_raw = ((uint16_t)raw[0] << 8) | raw[1];
    uint16_t h_raw = ((uint16_t)raw[3] << 8) | raw[4];

    out->temperature_c = -45.0f + 175.0f * ((float)t_raw / 65535.0f);
    out->humidity_pct = 100.0f * ((float)h_raw / 65535.0f);

    (void)shtc3_sleep();

    ESP_LOGI(TAG, "SHTC3: %.2f C, %.2f %%RH", out->temperature_c, out->humidity_pct);
    return ESP_OK;
}
