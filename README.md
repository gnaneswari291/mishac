# Crosswalk Request Beacon

A FirmGen Track B physical prototype: an ESP32 reads a pedestrian request button, activates a visible walk indicator for a fixed interval, and reports state changes over UART. It demonstrates a simple sense-decide-act loop on a real board.

> **Build status:** Frozen FirmGen PRD, runbook, ESP-IDF source scaffold, and an interactive browser simulation are present. There is no ESP32/STM32 board connected or available, and FirmGen/ESP-IDF are not available in this session. The simulation is not a qualifying physical prototype; FirmGen generation, compilation, flashing, and board acceptance testing remain incomplete.

## Track

- **Chosen track:** B — FirmGen.
- **Prototype:** ESP32 crosswalk request beacon.
- **Bonus:** No bonus claimed. Live Intelligence is optional and can be enabled in FirmGen if telemetry is configured and shown. Closed Loop is not claimed.

## Behavior

1. A pedestrian presses an external request button.
2. The ESP32 debounces the input and enters `WALK_ACTIVE` for 10 seconds.
3. An external LED blinks during the walk interval and is off otherwise.
4. UART logs the request, state, and timer completion so the acceptance behavior can be shown in FirmGen Monitor.

This is a tabletop demonstrator, not a traffic controller and not for connecting to roadway equipment.

## Hardware assumed by the PRD

- ESP32 development board (ESP-IDF v5.5), USB cable.
- Momentary pushbutton wired between GPIO 4 and GND; firmware uses the internal pull-up.
- LED with a suitable series resistor between GPIO 2 and GND (or adjust the pins to suit the board and kit).
- Common ground. Use only 3.3 V compatible GPIO wiring.

Check the actual board pinout before wiring. GPIO 2/4 are defaults in the PRD and may need changing for a specific board.

## FirmGen workflow

1. Upload [`docs/FirmGen_PRD.md`](docs/FirmGen_PRD.md) into FirmGen v0.4.1.
2. Review the generated requirements against the frozen PRD and resolve any inferred requirement that changes scope. Freeze the requirements before implementation.
3. Generate/implement, build with ESP-IDF v5.5, flash to the physical ESP32, and use Monitor to demonstrate UART output and LED behavior.
4. Run the acceptance checks in the PRD on the board and save the FirmGen traceability report.
5. Use FirmGen's Git view to push the actual generated project to this repository, or copy the FirmGen project into `firmware/` and preserve the traceability report in `evidence/`.

The detailed sequence is in [`docs/firmgen-demo-runbook.md`](docs/firmgen-demo-runbook.md).

## Evidence required for submission

- FirmGen traceability report linking each requirement to code and tests.
- FirmGen Monitor/UART output showing button request and state transitions.
- Photo/video or live venue demonstration showing the physical LED response.
- FirmGen project pushed through FirmGen Git view or included in `firmware/`.

Submission form from the supplied event instructions: https://forms.gle/bnocK9CNLndiZLwf8

## Repository contents

- `docs/FirmGen_PRD.md` — frozen product requirements to upload into FirmGen.
- `docs/firmgen-demo-runbook.md` — installation, wiring, build, flash, and evidence checklist.
- `firmware/` — ESP-IDF source scaffold; replace or reconcile with the actual FirmGen-generated project and preserve its tests/metadata.
- `simulation/index.html` — interactive browser-only demonstration; useful to illustrate behavior, but does not satisfy the physical-board requirement.
- `evidence/` — traceability report, UART/Monitor capture, and physical demo evidence.
