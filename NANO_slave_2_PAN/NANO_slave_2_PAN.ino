// NANO Slave 2 — Camera Pan axis
// Axis config only. Shared implementation: ../NANO_slave_shared/nano_slave_impl.h

#define AXIS_NAME "PAN"
constexpr bool AXIS_SERIAL_DEBUG = false;

///// SPEED STAGE SETTINGS
// Step pulse delay in microseconds per stage (0 = slowest, 4 = fastest).
constexpr int STAGE_COUNT = 5;
const     int STAGE_DELAYS[STAGE_COUNT] = {3000, 1700, 650, 300, 150};

///// RAMPING SETTINGS
constexpr int RAMP_INCREMENT   = 150;
constexpr int RAMP_START_DELAY = 6000;
constexpr int RAMP_STOP_DELAY  = 6000;

///// FEATURE FLAGS
#define HAS_SPEED_ADJ         1  // fine-tune speed adjustment pins enabled
#define STAGE_ZERO_CONTINUOUS 1  // stage 0->1 fires continuously while held

///// SPEED ADJUSTMENT (only used when HAS_SPEED_ADJ = 1)
constexpr int speedAdjUp       = 11;  // fine-tune: decrease pulse delay (faster)
constexpr int speedAdjDown     = 12;  // fine-tune: increase pulse delay (slower)
constexpr int HIGH_SPEED_DELAY = 100; // used when both adj pins are HIGH
constexpr int MIN_DELAY        = 100;
constexpr int MAX_DELAY        = 6000;

#include "nano_slave_impl.h"
