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
- XIAO GPIO22 -> SHTC3 SDA
- XIAO GPIO23 -> SHTC3 SCL
- XIAO 3.3V -> SHTC3 VCC
- XIAO GND -> SHTC3 GND

## Build and flash
Prerequisites:
- ESP-IDF `v5.5.x` installed locally
- Serial port access to `/dev/ttyACM0` (on Linux, user in `dialout`/`uucp`)

```bash
git clone https://github.com/twhite0011/zigbee-sensor-temp.git
cd zigbee-sensor-temp
source ~/esp/esp-idf/export.sh
idf.py set-target esp32c6
idf.py build
idf.py -p /dev/ttyACM0 flash monitor
```

If monitor fails in a non-interactive shell, run:

```bash
source ~/esp/esp-idf/export.sh
idf.py -p /dev/ttyACM0 flash
idf.py -p /dev/ttyACM0 monitor
```

## Zigbee2MQTT setup and pairing
1. Configure Zigbee2MQTT to load `z2m_converter.js` as an external converter in `configuration.yaml`:
   ```yaml
   external_converters:
     - /path/to/zigbee-sensor-temp/z2m_converter.js
   ```
2. Restart Zigbee2MQTT.
3. Enable `permit_join` in Zigbee2MQTT.
4. Flash/reboot device.
5. Wait for interview; device should expose temperature and humidity entities.

Expected runtime logs after join:
- `zigbee: Rejoined network PAN=... CH=... SHORT=...` (or joined network log)
- `sensors: SHTC3: <temp> C, <humidity> %RH`
- `zigbee: Reported temp=<temp> C humidity=<humidity> %`

## Notes
- Firmware currently uses Zigbee channel **11** only.
- If pairing fails, run `idf.py -p /dev/ttyACM0 erase-flash flash`.

## License
This project source is licensed under the MIT License. See `LICENSE`.

## Third-party components
`espressif/esp-zigbee-lib` and `espressif/esp-zboss-lib` are resolved by ESP-IDF Component Manager from `main/idf_component.yml` and are not required to be committed in this repository. They keep their own upstream licenses.
