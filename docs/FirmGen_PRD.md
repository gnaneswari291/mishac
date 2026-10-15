# FirmGen PRD: Crosswalk Request Beacon

**Version:** 1.0 — requirements frozen for FirmGen build  
**Track:** B — FirmGen  
**Target:** ESP32 development board using ESP-IDF v5.5  
**Purpose:** Demonstrate a physical sense-decide-act loop on a tabletop crosswalk beacon.

## Problem

A pedestrian needs a clear acknowledgement that a crosswalk request was received. This prototype uses a physical button as the request input and a visible LED as the walk indication. The ESP32 also reports state changes over UART for demonstration and debugging.

This is a tabletop educational demonstrator. It must not control or be connected to roadway equipment.

## Hardware and pin assumptions

- ESP32 development board, powered over USB.
- Momentary button between GPIO 4 and GND. Configure internal pull-up; pressed reads low.
- External LED and appropriate series resistor from GPIO 2 to GND; LED on means active. Change pin assignments if required by the actual board.
- All GPIO signals are 3.3 V compatible. Verify board pinout before wiring.

## Frozen requirements

| ID | Type | Requirement | Acceptance criteria |
|---|---|---|---|
| REQ-01 | Explicit | The firmware shall initialize the button input on GPIO 4 with an internal pull-up and the indicator output on GPIO 2 in a safe inactive state. | On boot, UART reports initialization and the LED is off. |
| REQ-02 | Explicit | The firmware shall debounce button input and recognize a press as one request event. | One deliberate press produces exactly one `REQUEST_ACCEPTED` event; holding the button does not repeatedly retrigger. |
| REQ-03 | Explicit | On a valid request while idle, firmware shall enter `WALK_ACTIVE` for 10 seconds. | UART reports `STATE=WALK_ACTIVE`; the measured interval is 10 seconds ±1 second. |
| REQ-04 | Explicit | During `WALK_ACTIVE`, firmware shall blink the LED at 1 Hz (500 ms on, 500 ms off). | LED visibly blinks throughout the active interval. |
| REQ-05 | Explicit | When the 10-second interval expires, firmware shall enter `IDLE` and turn the LED off. | UART reports `STATE=IDLE`; LED is off within 1 second of expiry. |
| REQ-06 | Explicit | Firmware shall write boot, accepted request, state transition, and timer completion messages to UART at 115200 baud. | Messages are readable in FirmGen Monitor/UART capture at 115200 baud. |
| REQ-07 | Explicit | Firmware shall remain responsive during the active interval; it shall not block button sampling or state updates with a 10-second delay. | A second button press during `WALK_ACTIVE` is logged as `REQUEST_IGNORED_BUSY`; the active timer continues. |
| REQ-08 | Explicit | The generated FirmGen project shall build for the selected ESP32 target with ESP-IDF v5.5 and flash to the physical board. | Successful build/flash output and board run are captured. |

## State behavior

- `BOOT` → configure GPIO/UART → `IDLE`.
- `IDLE` + debounced press → log `REQUEST_ACCEPTED` → `WALK_ACTIVE`.
- `WALK_ACTIVE` → blink LED without blocking and monitor elapsed time.
- `WALK_ACTIVE` + another debounced press → log `REQUEST_IGNORED_BUSY`; do not restart the timer.
- 10 seconds elapsed → LED off → log transition → `IDLE`.

## Non-goals

- No vehicle detection, camera analytics, wireless messaging, or road infrastructure control.
- No safety certification or claim that the signal is suitable for actual traffic use.
- Live Intelligence telemetry is optional and is not required for acceptance; UART logs are required.

## Acceptance test procedure

1. Flash firmware to a physical ESP32 board and open FirmGen Monitor/UART at 115200 baud.
2. Reset the board. Confirm boot/initialization message and LED off.
3. Press and release the button once. Confirm one `REQUEST_ACCEPTED`, `STATE=WALK_ACTIVE`, and visible LED blinking.
4. Press again during the active interval. Confirm `REQUEST_IGNORED_BUSY` and that the original timer is not extended.
5. Measure that the LED returns off and the UART reports `STATE=IDLE` after 10 seconds ±1 second.
6. Repeat the request once to confirm the system returns to a usable idle state.
7. Save UART/Monitor output and physical behavior evidence. Export FirmGen traceability report showing every REQ ID linked to implementation and tests.

## Submission evidence

- FirmGen traceability report.
- UART/Monitor output proving boot, request, active, ignored-busy, and idle transitions.
- Live physical demonstration or short video showing the button and LED.
- FirmGen project Git link/export and build/flash confirmation.
