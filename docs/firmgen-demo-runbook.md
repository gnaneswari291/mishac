# FirmGen build and live demo runbook

## Before building

1. Install FirmGen v0.4.1 and prerequisites from the CraftifAI developer portal; sign in.
2. Use an ESP32 board and confirm FirmGen is configured for ESP-IDF v5.5. (An STM32 board is also allowed by the event, but this PRD is specifically for ESP32.)
3. Wire the momentary button from GPIO 4 to GND and an LED with a suitable series resistor to GPIO 2 and GND. Verify the exact board pinout and 3.3 V GPIO limits before powering it.
4. Upload `FirmGen_PRD.md`. Review the generated requirement set against its frozen acceptance criteria. Resolve any changed or inferred requirements before freezing.

## Build and demonstrate

1. Ask FirmGen to implement the PRD and generate the requirement-linked code/tests.
2. Build for the exact ESP32 board using ESP-IDF v5.5. Fix build issues in FirmGen and keep the traceability links intact.
3. Flash the physical board over USB. Simulation alone does not satisfy the challenge.
4. Open FirmGen Monitor/UART at 115200 baud and reset the board.
5. Show the LED off at boot. Press the request button: demonstrate visible 1 Hz blinking for 10 seconds and the matching UART states.
6. Press during the active interval and show `REQUEST_IGNORED_BUSY`; verify the original timer is not restarted. Wait for `IDLE`, then demonstrate a second normal request.
7. Save the traceability report and Monitor/UART output. Capture a photo/video of the actual physical button and LED response.
8. Push the FirmGen project through FirmGen's Git view, or place its exported project under `firmware/` and link the repository in the submission form.

## Evidence checklist

- [ ] FirmGen traceability report linking every requirement to implementation and tests.
- [ ] UART/Monitor capture at 115200 baud showing boot and state transitions.
- [ ] Physical ESP32 demonstration showing button input and blinking LED output.
- [ ] FirmGen Git view link or project export in the repository.
- [ ] Project description accurately identifies the board, ESP-IDF version, behavior, and evidence.

Submission form from the supplied event instructions: https://forms.gle/bnocK9CNLndiZLwf8
