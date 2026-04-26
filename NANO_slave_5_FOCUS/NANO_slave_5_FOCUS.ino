// NANO Slave 5 — Camera Focus axis
// Axis config only. Shared implementation: ../NANO_slave_shared/nano_slave_impl.h

#define AXIS_NAME "FOCUS"
constexpr bool AXIS_SERIAL_DEBUG = false;

///// SPEED STAGE SETTINGS
// Step pulse delay in microseconds per stage (0 = slowest, 2 = fastest).
// Focus uses 3 stages (coarser range than motion axes).
constexpr int STAGE_COUNT = 3;
const     int STAGE_DELAYS[STAGE_COUNT] = {2500, 800, 100};

///// RAMPING SETTINGS
constexpr int RAMP_INCREMENT   = 120;
constexpr int RAMP_START_DELAY = 4000;
constexpr int RAMP_STOP_DELAY  = 4000;

///// FEATURE FLAGS
#define HAS_SPEED_ADJ         0  // no fine-tune pins on this axis
#define STAGE_ZERO_CONTINUOUS 0  // all stage transitions require a rising edge

#include "nano_slave_impl.h"
