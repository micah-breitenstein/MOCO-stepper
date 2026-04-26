// MOCO-Stepper: Boom Swing axis
// Flash this sketch to the Nano for the Swing axis.

#define AXIS_NAME "SWING"
constexpr bool AXIS_SERIAL_DEBUG = false;

constexpr int STAGE_COUNT = 5;
const     int STAGE_DELAYS[STAGE_COUNT] = {5000, 2500, 1000, 500, 250};

constexpr int RAMP_INCREMENT   = 150;
constexpr int RAMP_START_DELAY = 8000;
constexpr int RAMP_STOP_DELAY  = 8000;

#define HAS_SPEED_ADJ         0
#define STAGE_ZERO_CONTINUOUS 1

#include <MOCO_Stepper.h>
