# MOCO-Stepper

Arduino Nano firmware for stepper motor axis controllers on the MOCO camera jib rig.

Each axis runs on its own Arduino Nano. All axes share a single core implementation (`src/MOCO_Stepper.h`) — only the per-axis timing and feature config differs.

## Axes

| Sketch | Axis | Stages | Speed Adj |
|--------|------|--------|-----------|
| `examples/Swing` | Boom Swing | 5 | No |
| `examples/Pan` | Camera Pan | 5 | Yes |
| `examples/Lift` | Boom Lift | 5 | No |
| `examples/Tilt` | Camera Tilt | 5 | Yes |
| `examples/Focus` | Camera Focus | 3 | No |

## Hardware

- **MCU:** Arduino Nano (ATmega328P)
- **Driver:** Step/direction stepper driver (e.g. DRV8825, A4988)
- **Controller:** Receives direction and speed commands from the MOCO master (Arduino Mega)

### Pin Assignments (all axes)

| Pin | Function |
|-----|----------|
| 2 | Driver DIR |
| 4 | Driver PUL |
| 6 | Down command (from Mega) |
| 7 | Up command (from Mega) |
| 8 | Speed stage up |
| 9 | Speed stage down |
| 11 | Fine speed adjust up *(Pan, Tilt only)* |
| 12 | Fine speed adjust down *(Pan, Tilt only)* |

## Flashing a Nano

Each physical Nano gets flashed once with its axis sketch. Repeat these steps for each axis.

**Step 1 — Install the library**
Clone this repo into your Arduino `libraries/` folder, or add it via Sketch → Include Library → Add .ZIP Library.

**Step 2 — Open the axis sketch**
Arduino IDE: File → Examples → MOCO-Stepper → select your axis (e.g. `Swing`)

**Step 3 — Select board and port**
- Board: Arduino Nano
- Processor: ATmega328P
- Port: the USB port your Nano is connected to

**Step 4 — Upload**
Click Upload (→). The Nano is now flashed and ready for that axis.

**arduino-cli equivalent:**
```sh
arduino-cli compile --fqbn arduino:avr:nano --library /path/to/MOCO-stepper examples/Swing
arduino-cli upload -p /dev/cu.usbmodemXXXX --fqbn arduino:avr:nano --library /path/to/MOCO-stepper examples/Swing
```

Replace `Swing` with the axis name and `/dev/cu.usbmodemXXXX` with your port.

## How It Works

Each example sketch defines axis-specific config constants then `#include`s the shared core:

```cpp
#define AXIS_NAME "SWING"
constexpr int STAGE_COUNT = 5;
const     int STAGE_DELAYS[STAGE_COUNT] = {5000, 2500, 1000, 500, 250};
constexpr int RAMP_INCREMENT   = 150;
constexpr int RAMP_START_DELAY = 8000;
constexpr int RAMP_STOP_DELAY  = 8000;
#define HAS_SPEED_ADJ         0
#define STAGE_ZERO_CONTINUOUS 1

#include <MOCO_Stepper.h>
```

**Speed stages** — step pulse delay in microseconds; lower = faster. Stage 0 is stopped/resting, stage 1–N progressively faster.

**Ramping** — on start and stop, `currentDelay` ramps toward `RAMP_START_DELAY` / `RAMP_STOP_DELAY` incrementally to smooth acceleration and deceleration.

**Speed adjust** *(Pan, Tilt)* — two fine-tune pins let the operator trim target delay ±1 µs per loop tick. Both HIGH simultaneously triggers a high-speed bypass mode.

**Direction conflict** — if both up and down commands are HIGH simultaneously, motion is stopped and the conflict is logged.

## Adding a New Axis

1. Copy `examples/Swing/Swing.ino` to `examples/MyAxis/MyAxis.ino`
2. Set `AXIS_NAME`, `STAGE_COUNT`, `STAGE_DELAYS`, ramp values, and feature flags
3. Set `HAS_SPEED_ADJ 1` and add `speedAdjUp/Down` pin constants if needed

## Part of MOCO-jib

This repo is used as a submodule in [MOCO-jib](https://github.com/micah-breitenstein/MOCO-jib), the full camera jib control system.
