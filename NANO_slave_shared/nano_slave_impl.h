// nano_slave_impl.h — Shared NANO slave implementation for all MOCO-jib motor axes.
//
// #include this file at the bottom of an axis .ino after defining all config constants:
//
//   #define AXIS_NAME         "SWING"          // used in debug/conflict messages
//   constexpr bool AXIS_SERIAL_DEBUG = false;
//
//   constexpr int STAGE_COUNT = 5;
//   const     int STAGE_DELAYS[STAGE_COUNT] = {5000, 2500, 1000, 500, 250};
//
//   constexpr int RAMP_INCREMENT   = 150;
//   constexpr int RAMP_START_DELAY = 8000;
//   constexpr int RAMP_STOP_DELAY  = 8000;
//
//   // Set 1 to enable fine-tune speed adjustment pins (e.g. PAN, TILT)
//   #define HAS_SPEED_ADJ 0
//   #if HAS_SPEED_ADJ
//   constexpr int speedAdjUp       = 11;
//   constexpr int speedAdjDown     = 12;
//   constexpr int HIGH_SPEED_DELAY = 100;
//   constexpr int MIN_DELAY        = 100;
//   constexpr int MAX_DELAY        = 6000;
//   #endif
//
//   // Set 1 for continuous stage 0→1 on hold (all axes except FOCUS)
//   #define STAGE_ZERO_CONTINUOUS 1
//
//   #include "../NANO_slave_shared/nano_slave_impl.h"

#pragma once

#ifndef AXIS_NAME
  #error "AXIS_NAME must be defined before including nano_slave_impl.h"
#endif
#ifndef HAS_SPEED_ADJ
  #error "HAS_SPEED_ADJ must be defined before including nano_slave_impl.h"
#endif
#ifndef STAGE_ZERO_CONTINUOUS
  #error "STAGE_ZERO_CONTINUOUS must be defined before including nano_slave_impl.h"
#endif

///// PIN ASSIGNMENTS (common to all axes)
constexpr int driverDIR    = 2;  // stepper driver direction pin
constexpr int driverPUL    = 4;  // stepper driver pulse pin
constexpr int upButton     = 7;  // HIGH = positive direction command from Mega
constexpr int downButton   = 6;  // HIGH = negative direction command from Mega
constexpr int speedUpPin   = 8;  // HIGH = increase speed stage
constexpr int speedDownPin = 9;  // HIGH = decrease speed stage

///// STATE
int stage = 0;
int lastSpeedUp   = 0;
int lastSpeedDown = 0;
bool lastDirectionConflict = false;
unsigned long speedIndicatorUntilMs = 0;

int targetDelay  = STAGE_DELAYS[0];
int currentDelay = STAGE_DELAYS[0];
bool motionActive          = false;
bool pendingStop           = false;
unsigned long lastStepMicros   = 0;
unsigned long rampUpdateMicros = 0;

bool currentDirection       = true;
bool requestedDirection     = true;
bool directionChangePending = false;

///// DEBUG HELPERS

void debugLog(const char* message) {
  if (AXIS_SERIAL_DEBUG) {
    Serial.println(message);
  }
}

void debugLogStage(int stageValue) {
  if (AXIS_SERIAL_DEBUG) {
    Serial.print("STAGE");
    Serial.println(stageValue);
  }
}

void debugLogDelay(int delayValue) {
  if (AXIS_SERIAL_DEBUG) {
    Serial.print("DELAY");
    Serial.println(delayValue);
  }
}

void triggerSpeedIndicatorPulse() {
  speedIndicatorUntilMs = millis() + 120;
}

///// STEPPER

// Non-blocking stepper pulse using micros() timing.
// Returns true if a pulse was sent.
bool stepMotorNonBlocking(bool dirHigh, unsigned long nowMicros) {
  if (nowMicros - lastStepMicros >= (unsigned long)currentDelay) {
    digitalWrite(driverDIR, dirHigh ? HIGH : LOW);
    digitalWrite(driverPUL, LOW);
    delayMicroseconds(2);
    digitalWrite(driverPUL, HIGH);
    lastStepMicros = nowMicros;
    return true;
  }
  return false;
}

///// RAMPING

// Gradually move currentDelay toward targetDelay every ~1ms.
void updateRamping(unsigned long nowMicros) {
  if (nowMicros - rampUpdateMicros < 1000) {
    return;
  }
  rampUpdateMicros = nowMicros;

  if (currentDelay < targetDelay) {
    currentDelay = min(currentDelay + RAMP_INCREMENT, targetDelay);
  } else if (currentDelay > targetDelay) {
    currentDelay = max(currentDelay - RAMP_INCREMENT, targetDelay);
  }
}

void startMotion() {
  if (!motionActive) {
    motionActive = true;
    currentDelay = RAMP_START_DELAY;
    pendingStop = false;
    lastStepMicros = 0;
    rampUpdateMicros = 0;
    targetDelay = STAGE_DELAYS[stage];
    debugLog("MOTION START");
  }
}

void stopMotion() {
  if (motionActive && !pendingStop) {
    pendingStop = true;
    targetDelay = RAMP_STOP_DELAY;
    debugLog("MOTION STOP");
  }
}

///// SPEED STAGE

void updateSpeedStage(int speedUpRead, int speedDownRead) {
  if (lastSpeedDown == 0 && speedDownRead == 1 && stage > 0) {
    stage--;
    targetDelay = STAGE_DELAYS[stage];
    debugLogStage(stage);
    debugLogDelay(targetDelay);
    triggerSpeedIndicatorPulse();
  }

#if STAGE_ZERO_CONTINUOUS
  // Stage 0→1: fires continuously while held (allows quick entry from resting speed).
  if (stage == 0 && speedUpRead == 1) {
    stage = 1;
    targetDelay = STAGE_DELAYS[1];
    debugLog("STAGE1");
    debugLogDelay(targetDelay);
    triggerSpeedIndicatorPulse();
  } else
#endif
  if (lastSpeedUp == 0 && speedUpRead == 1 && stage < STAGE_COUNT - 1) {
    stage++;
    targetDelay = STAGE_DELAYS[stage];
    debugLogStage(stage);
    debugLogDelay(targetDelay);
    triggerSpeedIndicatorPulse();
  }

  lastSpeedUp   = speedUpRead;
  lastSpeedDown = speedDownRead;
}

///// SPEED ADJUST (optional fine-tune + high-speed bypass)

#if HAS_SPEED_ADJ
// Updates targetDelay directly; ramping then moves currentDelay toward it.
void applySpeedAdjust(int adjUpRead, int adjDownRead) {
  if (adjUpRead == 1 && adjDownRead == 1) {
    debugLog("HIGH SPEED MODE");
    targetDelay = HIGH_SPEED_DELAY;
    return;
  }
  if (adjUpRead == 1 && adjDownRead == 0) {
    debugLog("SPEED UP ADJUST");
    targetDelay = max(targetDelay - 1, MIN_DELAY);
  }
  if (adjDownRead == 1 && adjUpRead == 0) {
    debugLog("SPEED DOWN ADJUST");
    targetDelay = min(targetDelay + 1, MAX_DELAY);
  }
}
#endif

///// ARDUINO ENTRY POINTS

void setup() {
  if (AXIS_SERIAL_DEBUG) {
    Serial.begin(57600);
  }
  pinMode(driverDIR,    OUTPUT);
  pinMode(driverPUL,    OUTPUT);
  pinMode(speedUpPin,   INPUT);
  pinMode(speedDownPin, INPUT);
  pinMode(upButton,     INPUT);
  pinMode(downButton,   INPUT);
  pinMode(LED_BUILTIN,  OUTPUT);
#if HAS_SPEED_ADJ
  pinMode(speedAdjUp,   INPUT);
  pinMode(speedAdjDown, INPUT);
#endif
}

void loop() {
  bool ledOn = motionActive || (millis() < speedIndicatorUntilMs);
  digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);

  unsigned long nowMicros = micros();

  int upRead    = digitalRead(upButton);
  int downRead  = digitalRead(downButton);
  int speedUp   = digitalRead(speedUpPin);
  int speedDown = digitalRead(speedDownPin);

  ///// SPEED STAGE CHANGES
  updateSpeedStage(speedUp, speedDown);

#if HAS_SPEED_ADJ
  ///// SPEED ADJUSTMENTS + HIGH SPEED MODE
  int adjUp   = digitalRead(speedAdjUp);
  int adjDown = digitalRead(speedAdjDown);
  applySpeedAdjust(adjUp, adjDown);
#endif

  ///// DIRECTION CONFLICT CHECK
  bool directionConflict = (upRead == HIGH && downRead == HIGH);
  if (directionConflict) {
    if (!lastDirectionConflict) {
      debugLog(AXIS_NAME " CONFLICT: both commands active; movement skipped");
    }
    lastDirectionConflict = true;
    stopMotion();
    return;
  }
  lastDirectionConflict = false;

  ///// MOTION LOGIC
  bool commandActive = (upRead == HIGH || downRead == HIGH);

  if (!commandActive) {
    stopMotion();
  } else {
    requestedDirection = (upRead == HIGH);

    if (!motionActive) {
      currentDirection = requestedDirection;
      startMotion();
    } else if (!pendingStop && requestedDirection != currentDirection) {
      directionChangePending = true;
      stopMotion();
    }
  }

  if (motionActive) {
    updateRamping(nowMicros);

    if (pendingStop && currentDelay >= RAMP_STOP_DELAY) {
      motionActive = false;
      pendingStop = false;
      debugLog("MOTION STOPPED");

      if (directionChangePending && commandActive) {
        currentDirection = requestedDirection;
        directionChangePending = false;
        startMotion();
      } else {
        directionChangePending = false;
      }
      return;
    }

    if (!pendingStop && commandActive) {
      stepMotorNonBlocking(currentDirection, nowMicros);
    }
  }
}
