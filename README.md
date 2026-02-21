# zigbee-sensor-temp

Minimal ESP-IDF Zigbee end-device for **Seeed XIAO ESP32-C6** + **SHTC3**.

## Features
- Zigbee end-device (ZED) for Zigbee2MQTT/Home Assistant
- Reports:
  - Temperature (cluster `0x0402`)
  - Humidity (cluster `0x0405`)
- Reporting interval: **30 seconds**
- No external Zigbee2MQTT converter needed (standard clusters)

## Wiring (I2C)
- XIAO GPIO6 -> SHTC3 SDA
- XIAO GPIO7 -> SHTC3 SCL
- XIAO 3.3V -> SHTC3 VCC
- XIAO GND -> SHTC3 GND

## Build and flash
```bash
cd /home/nonya/gits/zigbee-sensor-temp
source /home/nonya/gits/esp-idf/export.sh
idf.py set-target esp32c6
idf.py build
idf.py -p /dev/ttyACM0 flash monitor
```

## Zigbee2MQTT pairing
1. Enable `permit_join` in Zigbee2MQTT.
2. Flash/reboot device.
3. Wait for interview; device should expose temperature/humidity entities.

## Notes
- Channel preference starts with channel 11 and falls back to all channels.
- If pairing fails, run `idf.py -p /dev/ttyACM0 erase-flash flash`.
