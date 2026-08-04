# CrOS — CrowBox Operating System

Arduino firmware for the [CrowBox](https://www.thecrowbox.com/), an open-source experimentation platform that uses Skinnerian reinforcement to train corvids (crows, magpies, and related birds). Birds progress through four training phases until they deposit a coin and receive a food reward.

This repository is based on **CrOS v0.99c** (October 2018) from the official CrowBox project, with local enhancements:

- **HX1838 / NEC IR remote** for phase selection and manual basket override (not in stock CrOS)
- **IR transmitter** that emits NEC event codes on perch / coin activity (for cameras or external listeners)
- On-board **LED matrix** phase / override display (Arduino UNO R4–style `Arduino_LED_Matrix`)
- Longer default basket-open window (`15 s` vs stock `5 s`)

Upstream project: [thecrowbox.com](https://www.thecrowbox.com/) · Wiki: [thecrowbox.com/wiki](https://thecrowbox.com/wiki/) · Software notes: [CrOS software](https://thecrowbox.com/wiki/doku.php?id=kit%3Acros_software)

## How CrowBox training works

Based on the official CrowBox protocol and this firmware’s phase implementations:

| Phase | Name (firmware) | Behavior |
|-------|-----------------|----------|
| **1** | Discovery & free feeding | Reward basket stays **open**. Food is freely available. Coin deposits are ignored. |
| **2** | Reward on arrival | Basket usually **closed**. Landing on the perch opens the basket for a timed window (introduces lid sound/motion while food is still “free”), then it closes safely. |
| **3** | Reward on deposit (training coins) | Basket opens only when a **coin deposit** is detected. Training coins are supplied by the machine (hardware config). |
| **4** | Reward on deposit | Same software rules as Phase 3. Training coins are **no longer** dispensed; birds must bring coins. Ideal steady-state for an urban CrowBox. |

Phase can be advanced with the physical phase button **or** the IR remote. The selected phase is **persisted in EEPROM**. The matrix shows Roman numerals I–IIII for the active phase (or `O` / `C` while the basket is manually locked open/closed).

## IR remote & transmitter (fork-specific)

Stock CrOS has **no** IR support. This fork uses the [Arduino-IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) library with an **HX1838-compatible** receiver (NEC protocol) and an IR LED transmitter.

### Hardware

| Function | Pin | Notes |
|----------|-----|--------|
| IR receiver (HX1838 data) | `5` | `INPUT_PIN_IR_RX` |
| IR transmitter LED | `6` | `OUTPUT_PIN_IR_TX`, 38 kHz NEC via `IrSender` |

### Remote control (receiver)

Decoded as **NEC**. Known commands (byte from the decoded NEC payload):

| Command | Typical button | Action |
|---------|----------------|--------|
| `0x45` | 1 | Jump to Phase 1 |
| `0x46` | 2 | Jump to Phase 2 |
| `0x47` | 3 | Jump to Phase 3 |
| `0x44` | 4 | Jump to Phase 4 |
| `0x08` | Left | **Manual override**: open basket and keep it open |
| `0x5A` | Right | **Manual override**: close basket and keep it closed |
| `0x16` | `*` | Exit manual override; restore basket behavior for current phase |

While manual override is active, training protocols skip automatic basket open/close, and phase changes from the remote are blocked until override is cleared with `*`.

> Tip: Button codes vary by remote. If your keys differ, read serial debug output when `CROS_USE_SERIAL_DEBUG` is enabled and update the command constants in `cros_core.cpp`.

### Event transmitter

On perch land / coin events the firmware can emit NEC-style codes for an external logger or camera trigger:

| Event | Code constant | Value |
|-------|---------------|--------|
| Perch | `IR_CODE_PERCH` | `0x00FF01FE` |
| Coin | `IR_CODE_COIN` | `0x00FF02FD` |

Sending briefly tears down / restarts the IR receiver so TX and RX can share the stack.

## Repository layout

| File | Role |
|------|------|
| `cros.ino` | Arduino entry points (`setup` / `loop`) + global `ArduinoLEDMatrix` |
| `cros_core.h` / `cros_core.cpp` | “The Core” — training protocols, servo, sensors, EEPROM, IR, matrix |
| `cros_constants.h` | Pins, servo angles, timing, training phases, error codes |
| `cros_types.h` | Shared types (`cros_time_t` as seconds float) |
| `license` | Full **CC BY-SA 4.0** text |

## Other hardware (as coded)

Pin map keeps Arduino **UNO-compatible** interrupt pins for perch and coin. Matrix API targets boards that provide `Arduino_LED_Matrix` (typically **UNO R4 WiFi**).

| Function | Pin | Notes |
|----------|-----|--------|
| Status LED | `13` | Blinks for errors / phase reports |
| Basket servo | `9` | Opens/closes the reward lid |
| Perch switch | `2` | `INPUT_PULLUP`; pressed = LOW |
| Coin sensor | `3` | `INPUT_PULLUP` + **FALLING** interrupt |
| Phase select | `4` | `INPUT_PULLUP`; pressed = LOW; polled (not interrupt) |
| IR RX | `5` | See above |
| IR TX | `6` | See above |

### Servo / basket safety

- Open: servo to `180°`; closed: `0°` (after a known midpoint park on boot).
- Closing uses **stepped motion** (`BASKET_CLOSE_NUM_STEPS` × `BASKET_CLOSE_STEP_DELAY_MS`) so birds get time to clear the lid.
- After open/close, the servo is **detached** to reduce buzz/whine.
- Basket typically stays open `BASKET_REMAIN_OPEN_DURATION` (**15 s** in this fork) after a reward.

### Coin handling

- Deposits are enqueued from a coin ISR; Phase 3/4 opens the basket once per queued coin.
- Manual IR override bypasses automatic open/close while active.

### Camera hooks (stubs)

`RecordVideo()` / `StopRecordingVideo()` are placeholders for a future camera/relay interface. The IR **event transmitter** is the main outward signalling path in this fork today.

## Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (or compatible toolchain)
- Libraries: `Servo`, `EEPROM`, [IRremote](https://www.arduino.cc/reference/en/libraries/irremote/), `Arduino_LED_Matrix`
- Board that matches wiring:
  - Interrupt pins on **2** (perch) and **3** (coin)
  - Matrix + IR pins as above (UNO R4 WiFi is the intended target for the matrix API)
- CrowBox mechanical assembly — see [overview docs](https://thecrowbox.com/wiki/doku.php?id=kit%3Aoverview_doc) and [BOM](https://thecrowbox.com/wiki/doku.php?id=kit%3Acrowbox2_v20_bom)
- HX1838 (or compatible) IR receiver + IR LED / driver on pins 5 and 6
- NEC remote whose codes match the table above (or remap in code)

## Build / flash

1. Install Arduino IDE libraries: **IRremote**, and ensure `Arduino_LED_Matrix` is available for your board package.
2. Open `cros.ino` (keep all `cros_*` files in the same folder).
3. Select the correct board and port.
4. Upload the sketch.

Optional serial debug (baud **57600**): in `cros_constants.h`, `#define CROS_USE_SERIAL_DEBUG` (keep `#undef` for quiet field deployment). Serial is especially useful when pairing a new remote.

### Training phase control

- **Button (pin 4):** cycles Phase 1 → 2 → 3 → 4 → 1 (same as stock CrOS).
- **IR remote:** jump directly to a phase (buttons 1–4) or lock the basket open/closed (Left/Right), then `*` to resume protocol control.
- EEPROM address `4` stores the phase after a `"CrOS"` header at addresses `0–3`. Fresh boards create that header on first boot.

## Changes in this fork (vs stock v0.99c)

Stock CrOS v0.99c: [CrowBox wiki download](https://thecrowbox.com/wiki/doku.php?id=kit%3Acros_software).

| Feature | Stock | This repo |
|---------|-------|-----------|
| Phase select | Pin-4 button only | Button **+ NEC IR remote** |
| Manual basket lock | No | IR Left/Right + matrix `O`/`C` |
| IR event TX (perch/coin) | No | Yes (`IR_CODE_PERCH` / `IR_CODE_COIN`) |
| LED matrix phase display | No | Yes (`UpdateMatrixDisplay`) |
| Basket open duration | 5 s | **15 s** |

Upstream credits, concept, and mechanical design remain with the CrowBox project / Joshua Klein and community; see [thecrowbox.com](https://www.thecrowbox.com/).

## Known limitations (from firmware comments)

- Uptime is tracked with a float of seconds derived from `millis()`; continuous runs ~40+ days can misbehave on rollover — periodic power cycles when refilling food/coins avoid this.
- Ideal loop pacing (`CROS_IDEAL_LOOP_MS` ≈ 20 ms) still soft-rate-limits the main loop for switch bounce tolerance.
- Camera API stubs are not implemented; use IR TX or add your own hook.
- IR TX briefly reinitializes the receiver after each send.
- Some earlier GitHub-only tweaks (aggressive coin sticky-contact recovery / door max-open failsafe) were superseded when this IR tree was restored as the working source of truth from `Desktop/Coding/Crowbox`.

## Safety & ethics

- Do **not** remove or accelerate the stepped lid-close behavior without understanding bird-safety implications.
- Manual IR override can hold the basket open indefinitely — use carefully around wildlife and power-loss scenarios.
- Deploy only where you can maintain the machine (food refill, weather, observation) and follow local wildlife guidelines.
- CrowBox is an experimentation platform for cooperative human–corvid interaction research, not a commercial coin-harvesting product.

## Community & further reading

- Site: https://www.thecrowbox.com/
- Wiki: https://thecrowbox.com/wiki/
- Training phases: https://thecrowbox.com/wiki/doku.php?id=kit%3Atraining_phases
- Software install guide: https://thecrowbox.com/wiki/doku.php?id=kit%3Asoftware_configuration
- Forum: [CrowBoxKit Google Group](https://groups.google.com/group/CrowBoxKit)
- TED background (Joshua Klein / crow intelligence): linked from the official CrowBox site

## License

Except where otherwise noted, this work is licensed under **Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)**. See the `license` file and [creativecommons.org/licenses/by-sa/4.0/](https://creativecommons.org/licenses/by-sa/4.0/).

When you Share adapted material, keep attribution to the CrowBox / CrOS authors and apply a compatible ShareAlike license.
