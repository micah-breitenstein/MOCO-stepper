// NANO Slave 4 — Camera Tilt axis
// Axis config only. Shared implementation: ../NANO_slave_shared/nano_slave_impl.h
//
// Direction note: the "up" input pin triggers TILT DOWN and vice versa.
// This matches the Mega wiring convention where lift-up also sends tilt-down
// to counteract boom movement.

#define AXIS_NAME "TILT"
constexpr bool AXIS_SERIAL_DEBUG = false;

///// SPEED STAGE SETTINGS
// Step pulse delay in microseconds per stage (0 = slowest, 4 = fastest).
constexpr int STAGE_COUNT = 5;
const     int STAGE_DELAYS[STAGE_COUNT] = {9000, 4200, 1900, 1075, 400};

///// RAMPING SETTINGS
constexpr int RAMP_INCREMENT   = 200;
constexpr int RAMP_START_DELAY = 12000;
constexpr int RAMP_STOP_DELAY  = 12000;

///// FEATURE FLAGS
#define HAS_SPEED_ADJ         1  // fine-tune speed adjustment pins enabled
#define STAGE_ZERO_CONTINUOUS 1  // stage 0->1 fires continuously while held

///// SPEED ADJUSTMENT (only used when HAS_SPEED_ADJ = 1)
constexpr int speedAdjUp       = 11;   // fine-tune: decrease pulse delay (faster)
constexpr int speedAdjDown     = 12;   // fine-tune: increase pulse delay (slower)
constexpr int HIGH_SPEED_DELAY = 100;  // used when both adj pins are HIGH
constexpr int MIN_DELAY        = 100;
constexpr int MAX_DELAY        = 16000;

#include "nano_slave_impl.h"
