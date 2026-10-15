# Firmware project location

This folder now contains a manually prepared ESP-IDF v5.5 firmware scaffold for the frozen PRD. It is not FirmGen-generated, and must be imported/recreated through FirmGen to satisfy the challenge's PRD-driven workflow and generate traceability evidence. Keep FirmGen's generated tests and project metadata when doing that.

## Expected board wiring

- Button: GPIO 4 to GND (internal pull-up enabled).
- LED: GPIO 2 through a suitable series resistor to GND (active high).
- USB powers the ESP32. Verify the target board pinout before wiring.

## Build on a machine with ESP-IDF v5.5

From this folder in an ESP-IDF v5.5 terminal:

```bash
idf.py set-target esp32
idf.py build
idf.py -p <serial-port> flash monitor
```

This source has not been compiled or flashed in this session because FirmGen, ESP-IDF, and a board are unavailable here. After using FirmGen, use its project and traceability artifacts as the submission source of truth.
