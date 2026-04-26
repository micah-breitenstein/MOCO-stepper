// MOCO-Stepper: Camera Focus axis
// Flash this sketch to the Nano for the Focus axis.

#define AXIS_NAME "FOCUS"
constexpr bool AXIS_SERIAL_DEBUG = false;

constexpr int STAGE_COUNT = 3;
const     int STAGE_DELAYS[STAGE_COUNT] = {2500, 800, 100};

constexpr int RAMP_INCREMENT   = 120;
constexpr int RAMP_START_DELAY = 4000;
constexpr int RAMP_STOP_DELAY  = 4000;

#define HAS_SPEED_ADJ         0
#define STAGE_ZERO_CONTINUOUS 0

#include <MOCO_Stepper.h>
