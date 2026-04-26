// MOCO-Stepper: Camera Pan axis
// Flash this sketch to the Nano for the Pan axis.

#define AXIS_NAME "PAN"
constexpr bool AXIS_SERIAL_DEBUG = false;

constexpr int STAGE_COUNT = 5;
const     int STAGE_DELAYS[STAGE_COUNT] = {3000, 1700, 650, 300, 150};

constexpr int RAMP_INCREMENT   = 150;
constexpr int RAMP_START_DELAY = 6000;
constexpr int RAMP_STOP_DELAY  = 6000;

#define HAS_SPEED_ADJ         1
#define STAGE_ZERO_CONTINUOUS 1

constexpr int speedAdjUp       = 11;
constexpr int speedAdjDown     = 12;
constexpr int HIGH_SPEED_DELAY = 100;
constexpr int MIN_DELAY        = 100;
constexpr int MAX_DELAY        = 6000;

#include <MOCO_Stepper.h>
