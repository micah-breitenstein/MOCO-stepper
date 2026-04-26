// MOCO-Stepper: Camera Tilt axis
// Flash this sketch to the Nano for the Tilt axis.
// Direction note: "up" input triggers TILT DOWN (Mega wiring convention).

#define AXIS_NAME "TILT"
constexpr bool AXIS_SERIAL_DEBUG = false;

constexpr int STAGE_COUNT = 5;
const     int STAGE_DELAYS[STAGE_COUNT] = {9000, 4200, 1900, 1075, 400};

constexpr int RAMP_INCREMENT   = 200;
constexpr int RAMP_START_DELAY = 12000;
constexpr int RAMP_STOP_DELAY  = 12000;

#define HAS_SPEED_ADJ         1
#define STAGE_ZERO_CONTINUOUS 1

constexpr int speedAdjUp       = 11;
constexpr int speedAdjDown     = 12;
constexpr int HIGH_SPEED_DELAY = 100;
constexpr int MIN_DELAY        = 100;
constexpr int MAX_DELAY        = 16000;

#include <MOCO_Stepper.h>
