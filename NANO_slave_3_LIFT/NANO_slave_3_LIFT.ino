// NANO Slave 3 — Boom Lift axis
// Axis config only. Shared implementation: ../NANO_slave_shared/nano_slave_impl.h

#define AXIS_NAME "LIFT"
constexpr bool AXIS_SERIAL_DEBUG = false;

///// SPEED STAGE SETTINGS
// Step pulse delay in microseconds per stage (0 = slowest, 4 = fastest).
constexpr int STAGE_COUNT = 5;
const     int STAGE_DELAYS[STAGE_COUNT] = {5000, 2500, 1000, 500, 250};

///// RAMPING SETTINGS
constexpr int RAMP_INCREMENT   = 150;
constexpr int RAMP_START_DELAY = 8000;
constexpr int RAMP_STOP_DELAY  = 8000;

///// FEATURE FLAGS
#define HAS_SPEED_ADJ         0  // no fine-tune pins on this axis
#define STAGE_ZERO_CONTINUOUS 1  // stage 0->1 fires continuously while held

#include "nano_slave_impl.h"
