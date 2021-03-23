// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// North1               motor         1
// South2               motor         2
// West3L               motor         3
// East4R               motor         4
// LaserL               distance      10
// BorderDetector       line          B
// StopButton           bumper        C
// LaserR               distance      11
// ---- END VEXCODE CONFIGURED DEVICES ----
#include<vex.h>
#include<iostream>

using namespace vex;
// ---- Initialization ---- //
void spinMotors();
void updateVelocity(int n, bool useIn);
void eBrake();
void unbrake();
void updateConsole();
void checkLine();
void checkObject();
void screenColor(color c);
float LaserAvg(distanceUnits units);
bool laserInRange(int range, distanceUnits units);
bool degMove(int dir, float in, float deg, int v);

int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, range;
float northV, southV, eastV, westV, stickRotate, stickForward, stickSideways, autoTurnSpeed, autoFollowSpeed, autoFocus, charge;
bool init = true, useController, autonomous, objLeft, braking, atLine, nearObject, seeObject, brakeMem, autoMem, lineMem, detectMem, objMem, leftMem, inRange;

// ---- FUNCTIONS ---- //
void spinMotors() {
  // Spin wheels to preset velocity
  North1.spin(forward);
  South2.spin(forward);
  West3L.spin(forward);
  East4R.spin(forward);
}
void updateVelocity(int n, bool useIn) {
  //Calculate correct wheel speeds
  northV = (stickForward + stickSideways) + stickRotate;
  eastV = (stickForward - stickSideways) - stickRotate;
  westV = (stickForward - stickSideways) + stickRotate;
  southV = (stickForward + stickSideways) - stickRotate;
  //Set velocity
  if (!useIn) { //!useIn = use controller input
    North1.setVelocity(northV, percent);
    South2.setVelocity(southV, percent);
    West3L.setVelocity(westV, percent);
    East4R.setVelocity(eastV, percent);

  }
  else if (useIn && n != 0) {
    North1.setVelocity(n, percent);
    South2.setVelocity(n, percent);
    West3L.setVelocity(n, percent);
    East4R.setVelocity(n, percent);

  }
  else if (useIn && n == 0) {
    North1.stop();
    South2.stop();
    West3L.stop();
    East4R.stop();
  }
}
void eBrake() { //
  updateVelocity(0, true); //Halt
  North1.setStopping(hold);
  South2.setStopping(hold);
  West3L.setStopping(hold);
  East4R.setStopping(hold);
}
void unbrake() {
  North1.setStopping(coast);
  South2.setStopping(coast);
  West3L.setStopping(coast);
  East4R.setStopping(coast);
}
float laserAvg(distanceUnits units) {
  return (LaserL.objectDistance(units) + LaserR.objectDistance(units)) / 2;
}
void checkLine() {
  if (BorderDetector.value(percent) <= 10) {
    atLine = true;
  }
  else {
    atLine = false;
  }
}
void checkObject() {
  if (laserAvg(mm) > 1 && laserAvg(mm) < 100) {
    nearObject = true;
  }
  else {
    nearObject = false;
  }
}
bool laserInRange(int range, distanceUnits units) {
  if ((LaserL.objectDistance(units) <= range && LaserL.objectDistance(mm) > 1) || (LaserR.objectDistance(units) <= range && LaserR.objectDistance(mm) > 1)) {
    return true;
  }
  else {
    return false;
  }
}
void updateConsole() {
  if (init || braking != brakeMem || autonomous != autoMem || atLine != lineMem || laserInRange(range, mm) != detectMem || nearObject != objMem || objLeft != leftMem) {
    Brain.Screen.clearScreen(); //Clear Text
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.setFillColor(black);
    if (braking) {
      Brain.Screen.print("Braking: True\n");
    }
    else {
      Brain.Screen.print("Braking: False\n");
    }
    Brain.Screen.newLine();
    if (autonomous) {
      Brain.Screen.print("Autonomous: True\n");
    }
    else {
      Brain.Screen.print("Autonomous: False\n");
    }
    Brain.Screen.newLine();
    if (atLine) {
      Brain.Screen.print("At Line: True\n");
    }
    else {
      Brain.Screen.print("At Line: False\n");
    }
    Brain.Screen.newLine();
    if (laserInRange(range, mm)) {
      Brain.Screen.print("Detects Object: True\n");
    }
    else {
      Brain.Screen.print("Detects Object: False\n");
    }
    Brain.Screen.newLine();
    if (nearObject) {
      Brain.Screen.print("Near Object: True\n");
    }
    else {
      Brain.Screen.print("Near Object: False\n");
    }
    Brain.Screen.newLine();
    if (objLeft) {
      Brain.Screen.print("Object is Left\n");
    }
    else {
      Brain.Screen.print("Object is Right\n");
    }
    

    init = false;
    brakeMem = braking;
    autoMem = autonomous;
    lineMem = atLine;
    detectMem = laserInRange(range, mm);
    objMem = nearObject;
    leftMem = objLeft;
  }
}
bool degMove(int dir, float in, float deg, int v) {
  updateVelocity(v, true);
  if (dir == 1) { //Forward
    float wheelDeg = in / 0.05;
    North1.spinFor(wheelDeg, degrees, false);
    South2.spinFor(wheelDeg, degrees, false);
    West3L.spinFor(wheelDeg, degrees, false);
    East4R.spinFor(wheelDeg, degrees, true);
  }
  else if (dir == 2) { //Backward
    float wheelDeg = in / -0.05;
    North1.spinFor(wheelDeg, degrees, false);
    South2.spinFor(wheelDeg, degrees, false);
    West3L.spinFor(wheelDeg, degrees, false);
    East4R.spinFor(wheelDeg, degrees, true);
  }
  else if (dir == 3) { //Left
    float wheelDeg = in / 0.05;
    North1.spinFor(wheelDeg * -1, degrees, false);
    South2.spinFor(wheelDeg * -1, degrees, false);
    West3L.spinFor(wheelDeg, degrees, false);
    East4R.spinFor(wheelDeg, degrees, true);
  }
  else if (dir == 4) { //Right
    float wheelDeg = in / 0.05;
    North1.spinFor(wheelDeg, degrees, false);
    South2.spinFor(wheelDeg, degrees, false);
    West3L.spinFor(wheelDeg * -1, degrees, false);
    East4R.spinFor(wheelDeg * -1, degrees, true);
  }
  else if (dir == 5) { //Clockwise
    float wheelDeg = deg / 0.279;
    North1.spinFor(wheelDeg, degrees, false);
    South2.spinFor(wheelDeg * -1, degrees, false);
    East4R.spinFor(wheelDeg * -1, degrees, false);
    West3L.spinFor(wheelDeg, degrees, true);
  }
  else if (dir == 6) { //Counter-Clockwise
    float wheelDeg = deg / 0.279;
    North1.spinFor(wheelDeg * -1, degrees, false);
    South2.spinFor(wheelDeg, degrees, false);
    East4R.spinFor(wheelDeg, degrees, false);
    West3L.spinFor(wheelDeg * -1, degrees, true);
  }
  return true;
}
void screenColor(color c) {
  Brain.Screen.setFillColor(c);
  Brain.Screen.drawRectangle(270, 0, 210, 272);
  updateConsole();
}
void updateDirection(int r) {
  if (LaserL.objectDistance(mm) <= r && (LaserR.objectDistance(mm) > LaserL.objectDistance(mm) + 20)) { //Only LaserL sees object
    objLeft = true;
    stickRotate = autoFocus * -1; //Rotate left
  }
  else if (LaserR.objectDistance(mm) <= r && (LaserL.objectDistance(mm) > LaserR.objectDistance(mm) + 20)) { //Only LaserR sees object
    objLeft = false;
    stickRotate = autoFocus; //Rotate right
  }
}
// ===========================================================================================================

// ---- MAIN DRIVE CONTROL ---- //
int omniControl() {
  //Autonomous speeds
  autoFollowSpeed = 25.0; //Object track/approach speed
  autoTurnSpeed   = 20.0; //Looking for object spin speed
  autoFocus = 15;   //Correct to center strength
  charge = 50; //nearObject charge velocity
  range = 800;
  atLine = false, nearObject = false, autonomous = false;
  //Forever
  while (true) { //Run Forever
    checkLine(); //Check if over boundary
    checkObject(); //Check if near object
    
    
    

    if (StopButton.pressing()) { //Emergency stop button
      braking = true;
      autonomous = false;
      Controller1.rumble(rumbleShort);
    }

    if (!braking) { //Not breaking
      unbrake(); //Release wheels
      if (autonomous) { //If bot is in autonomous mode
        if (!atLine && !nearObject) { //Not over line AND not near object
          //screenColor(orange); //Set cortex color to ORANGE
          
          if (laserInRange(range, mm)) { //Object obstructs either laser
            screenColor(yellow);
            updateDirection(range);
            seeObject = true;
            if (LaserL.objectDistance(mm) <= range && (LaserR.objectDistance(mm) > 500)) { //Only LaserL sees object
              objLeft = true;
              stickRotate = autoFocus * -1; //Rotate left
            }
            if (LaserR.objectDistance(mm) <= range && (LaserL.objectDistance(mm) > 500)) { //Only LaserR sees object
              objLeft = false;
              stickRotate = autoFocus; //Rotate right
            }
            else {
              stickRotate = 0;
            }
            
            stickForward = autoFollowSpeed; //Track object at autoFollowSpeed velocity
          }
          
          
          
          else { //If object not in range
            screenColor(orange); //Set cortex color to ORANGE
            seeObject = false;
            updateConsole();
            stickForward = 0.0;
            
            if (objLeft) {
              stickRotate = autoTurnSpeed * -1.0; //Rotate left based on last known object direction
            }
            
            else {
              stickRotate = autoTurnSpeed; //Rotate right based on last known object direction
            }
          }
          
          
          updateVelocity(1, false);
          spinMotors();
        
        }
        else if (atLine) { //If robot over line
          screenColor(white); //Set cortex color to WHITE
          degMove(5, 0, 180, 50); //Spin 180deg with a velocity of 50
          degMove(1, 7, 0, 40); //Move forward 7in with a velocity of 40
          autonomous = false;
        }
        else if (nearObject && charge > 1) {  //If near object AND charge velocity set
          if (BorderDetector.value(percent) <= 10) { //Check if at arena border or robot picked up
            screenColor(white); //Set cortex color to WHITE
            braking = true;
          }
          else { //Charge sequence
            screenColor(purple); //Set cortex color to PURPLE
            updateVelocity(charge, true); //Set velocity to charge velocity
            spinMotors(); //Update motor spin
          }
        }
        else if (nearObject && charge == 0) { //If near object AND no charge velocity set
          updateVelocity(0, true); //Stop
          braking = true;
        }
      }
      else { //Use controller input
        screenColor(green); //Set cortex color to GREEN
        stickRotate = Controller1.Axis1.position();
        stickForward = Controller1.Axis3.position();
        stickSideways = Controller1.Axis4.position();
        updateVelocity(1, false); //Update velocity by stick input
        spinMotors();
      }
    }
    else if (braking) {
      screenColor(red); //Set cortex color to GREEN
      eBrake();
    }
    wait(5, msec);
  }
  
  return 0;
}

// ---- EVENT TRIGGERS ---- //
void onEvent_ButtonL1Pressed() { //L1 Pressed
  braking = true;
}
void onEvent_ButtonL1Released() { //L1 Released
  braking = false;
  unbrake();
}
void onEvent_ButtonR1Pressed() { //R1 Pressed
  //Toggle autonomous mode
  autonomous = !autonomous;
}

// ---- REGISTER EVENT HANDLERS ---- // -- INT MAIN -- //
int main() {
  Controller1.ButtonL1.pressed(onEvent_ButtonL1Pressed);
  Controller1.ButtonL1.released(onEvent_ButtonL1Released);
  Controller1.ButtonR1.pressed(onEvent_ButtonR1Pressed);

  wait(15, msec);

  screenColor(green);
  omniControl();
}
