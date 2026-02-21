# zigbee-sensor-temp

Minimal ESP-IDF Zigbee end-device for **Seeed XIAO ESP32-C6** + **SHTC3**.

## Features
- Zigbee end-device (ZED) for Zigbee2MQTT/Home Assistant
- Reports:
  - Temperature (cluster `0x0402`)
  - Humidity (cluster `0x0405`)
- Reporting interval: **30 seconds**
- Includes external Zigbee2MQTT converter: `z2m_converter.js`

## Wiring (I2C)
- XIAO GPIO6 -> SHTC3 SDA
- XIAO GPIO7 -> SHTC3 SCL
- XIAO 3.3V -> SHTC3 VCC
- XIAO GND -> SHTC3 GND

## Build and flash
Prerequisite: ESP-IDF is installed on your system.

```bash
git clone https://github.com/twhite0011/zigbee-sensor-temp.git
cd zigbee-sensor-temp
source /path/to/esp-idf/export.sh
idf.py set-target esp32c6
idf.py build
idf.py -p /dev/ttyACM0 flash monitor
```

## Zigbee2MQTT setup and pairing
1. Configure Zigbee2MQTT to load `z2m_converter.js` as an external converter.
2. Restart Zigbee2MQTT.
3. Enable `permit_join` in Zigbee2MQTT.
4. Flash/reboot device.
5. Wait for interview; device should expose temperature and humidity entities.

## Notes
- Channel preference starts with channel 11 and falls back to all channels.
- If pairing fails, run `idf.py -p /dev/ttyACM0 erase-flash flash`.

## License
This project source is licensed under the MIT License. See `LICENSE`.

## Third-party components
`espressif/esp-zigbee-lib` and `espressif/esp-zboss-lib` are resolved by ESP-IDF Component Manager from `main/idf_component.yml` and are not required to be committed in this repository. They keep their own upstream licenses.
