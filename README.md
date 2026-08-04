# CrOS — CrowBox Operating System

Arduino firmware for the [CrowBox](https://www.thecrowbox.com/), an open-source experimentation platform that uses Skinnerian reinforcement to train corvids (crows, magpies, and related birds). Birds progress through four training phases until they deposit a coin and receive a food reward.

This repository is based on **CrOS v0.99c** (October 2018) from the official CrowBox project, with local enhancements for coin-sensor reliability, door safety timeouts, and an on-board LED matrix phase display.

Upstream project: [thecrowbox.com](https://www.thecrowbox.com/) · Wiki: [thecrowbox.com/wiki](https://thecrowbox.com/wiki/) · Software notes: [CrOS software](https://thecrowbox.com/wiki/doku.php?id=kit%3Acros_software)

## How CrowBox training works

Based on the official CrowBox protocol and this firmware’s phase implementations:

| Phase | Name (firmware) | Behavior |
|-------|-----------------|----------|
| **1** | Discovery & free feeding | Reward basket stays **open**. Food is freely available. Coin deposits are ignored. |
| **2** | Reward on arrival | Basket usually **closed**. Landing on the perch opens the basket for a timed window (introduces lid sound/motion while food is still “free”), then it closes safely. |
| **3** | Reward on deposit (training coins) | Basket opens only when a **coin deposit** is detected. Training coins are supplied by the machine (hardware config). |
| **4** | Reward on deposit | Same software rules as Phase 3. Training coins are **no longer** dispensed; birds must bring coins. Ideal steady-state for an urban CrowBox. |

Phase selection advances with a pushbutton and is **persisted in EEPROM** so it survives power cycles. The indicator LED blinks N times for Phase N after a change; this fork also renders Roman-numeral-style patterns (I–IIII) on the Arduino LED matrix.

## Repository layout

| File | Role |
|------|------|
| `cros.ino` | Arduino entry points (`setup` / `loop`) hosting the global `CCrowboxCore` instance |
| `cros_core.h` / `cros_core.cpp` | “The Core” — training protocols, servo, sensors, EEPROM, matrix display |
| `cros_constants.h` | Pins, servo angles, timing, training phases, error codes |
| `cros_types.h` | Shared types (`cros_time_t` as seconds float) |
| `license` | Full **CC BY-SA 4.0** text |

## Hardware (as coded)

Pin map assumes Arduino **UNO-compatible** interrupt-capable pins for perch and coin (classic UNO layout). The LED matrix API (`Arduino_LED_Matrix`) targets boards that expose that library (typically **UNO R4 WiFi**).

| Function | Pin | Notes |
|----------|-----|--------|
| Status LED | `13` | Blinks for errors, phase reports, coin/perch debug flashes |
| Basket servo | `9` | Opens/closes the reward lid |
| Perch switch | `2` | `INPUT_PULLUP`; pressed = LOW |
| Coin sensor | `3` | `INPUT_PULLUP` + **FALLING** interrupt |
| Phase select | `4` | `INPUT_PULLUP`; pressed = LOW; polled (not interrupt) |

### Servo / basket safety

- Open: servo to `180°`; closed: `0°` (after a known midpoint park on boot).
- Closing uses **stepped motion** (`BASKET_CLOSE_NUM_STEPS` × `BASKET_CLOSE_STEP_DELAY_MS`) so birds get time to clear the lid.
- After open/close, the servo is **detached** to reduce buzz/whine.
- Basket typically stays open `BASKET_REMAIN_OPEN_DURATION` (5 s) after a reward.
- Failsafe: force-close if the door has been open longer than `DOOR_MAX_OPEN_MS` (30 s).

### Coin handling

- Deposits are enqueued in an ISR with debounce; Phase 3/4 later open the basket once per queued coin (up to a small queue).
- Extra logic in this fork: stuck-LOW detection, periodic interrupt re-attach, `ResetCoinSensor()`, and blinking the LED outside the ISR when a coin is accepted.

### Camera hooks (stubs)

`RecordVideo()` / `StopRecordingVideo()` are placeholders for a future camera/relay interface. They do not control hardware yet (same as upstream release notes).

## Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (or compatible toolchain)
- Board that matches your wiring:
  - **Classic CrowBox:** Arduino UNO / UNO-compatible with interrupt pins on 2 & 3
  - **This fork’s matrix display:** board supporting `Arduino_LED_Matrix` + `Servo` + `EEPROM`
- CrowBox mechanical assembly (housing, servo, perch/coin switches, reward basket) — see the [overview docs](https://thecrowbox.com/wiki/doku.php?id=kit%3Aoverview_doc) and [BOM](https://thecrowbox.com/wiki/doku.php?id=kit%3Acrowbox2_v20_bom)

## Build / flash

1. Install the Arduino IDE and open `cros.ino` (keep all `cros_*.h` / `cros_core.cpp` in the same folder).
2. Select the correct board and port.
3. Ensure libraries resolve: `Servo`, `EEPROM`, and `Arduino_LED_Matrix` (matrix boards).
4. Upload the sketch.

Optional serial debug (baud **57600**): in `cros_constants.h`, `#define CROS_USE_SERIAL_DEBUG` (keep it `#undef` for field deployment).

### Training phase control

- Press the phase-select button (pin 4) to cycle Phase 1 → 2 → 3 → 4 → 1.
- EEPROM address `4` stores the phase after a `"CrOS"` header at addresses `0–3`.
- First boot on a fresh board creates that EEPROM header automatically.

## Changes in this fork (vs stock v0.99c)

Stock CrOS v0.99c lives on the [CrowBox wiki download](https://thecrowbox.com/wiki/doku.php?id=kit%3Acros_software). Notable differences here:

- **Arduino LED matrix** phase indicator (`UpdatePhaseDisplay`)
- Stronger **coin-sensor** debounce / stuck-contact recovery and queue limits
- **Door failsafe** maximum open time and minimum reopen spacing
- LED blink feedback for coin/perch events moved out of the ISR where needed

Upstream credits, concept, and mechanical design remain with the CrowBox project / Joshua Klein and community; see [thecrowbox.com](https://www.thecrowbox.com/).

## Known limitations (from firmware comments)

- Uptime is tracked with a float of seconds derived from `millis()`; continuous runs ~40+ days can misbehave on rollover — periodic power cycles when refilling food/coins avoid this.
- Ideal loop pacing (`CROS_IDEAL_LOOP_MS` ≈ 20 ms) still soft-rate-limits the main loop for switch bounce tolerance.
- Camera integration is not implemented.

## Safety & ethics

- Do **not** remove or accelerate the stepped lid-close behavior without understanding bird-safety implications.
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
