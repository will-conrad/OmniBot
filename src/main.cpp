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
float LaserAvg(distanceUnits units);
void checkLine();
void checkObject();
void updateConsole();
bool degMove(int dir, float in, float deg, int v);
void screenColor(color c);

int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, Vision13_objectIndex = 0;
float northV, southV, eastV, westV, stickRotate, stickForward, stickSideways, objectLoc, autoTurnSpeed, autoFollowSpeed, autoFocus, charge;
bool useController, autonomous, running, objectExists, objLeft, braking, atLine, nearObject, seeObject, brakeMem, autoMem, lineMem, objMem;

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
  if (BorderDetector.reflectivity() <= 10) {
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
  if ((LaserL.objectDistance(units) <= range && LaserL.isObjectDetected()) || (LaserR.objectDistance(units) <= range && LaserR.isObjectDetected())) {
    return true;
  }
  else {
    return false;
  }
}
void updateConsole() {
  if (braking != brakeMem || autonomous != autoMem || atLine != lineMem || nearObject != objMem) {
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
    if (nearObject) {
      Brain.Screen.print("Near Object: True\n");
    }
    else {
      Brain.Screen.print("Near Object: False\n");
    }

    brakeMem = braking;
    autoMem = autonomous;
    lineMem = atLine;
    objMem = nearObject;
  }
}

void screenColor(color c) {
  Brain.Screen.setFillColor(c);
  Brain.Screen.drawRectangle(270, 0, 210, 272);
  updateConsole();
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

// ===========================================================================================================

// ---- MAIN DRIVE CONTROL ---- //
int omniControl() {
  //Autonomous speeds
  autoFollowSpeed = 20.0;
  autoTurnSpeed   = 20.0;
  autoFocus = 10;   //Correct to center robot
  charge = 50;
  atLine = false;
  nearObject = false;
  
  autonomous = false;
  //Forever
  while (true) {
    checkLine(); 
    checkObject();
    
    if (StopButton.pressing()) {
      braking = true;
      autonomous = false;
      Controller1.rumble(rumbleShort);
    }
    if (!braking) {
      unbrake();
      if (autonomous) { //If bot is in auto mode
        
        if (!atLine && !nearObject) { //NOT at line
          screenColor(orange); //Orange if autonomous

          if (laserInRange(750, mm)) { //Sees object
            // || (Laser.objectDistance(mm) < 700 && Laser.objectDistance(mm) > 2)
            seeObject = true;
            
            if (LaserL.objectDistance(mm) <= 700 && (LaserR.objectDistance(mm) > 700 || LaserR.isObjectDetected() == false)) { //Determine where object is
              objLeft = true;
              stickRotate = autoFocus * -1;
            }
            else if (LaserR.objectDistance(mm) <= 700 && (LaserL.objectDistance(mm) > 700 || LaserL.isObjectDetected() == false)){
              objLeft = false;
              stickRotate = autoFocus;
            }
            stickForward = autoFollowSpeed; //Track object
          }
          else { //Didnt see object
            seeObject = false;
            stickForward = 0.0;
            if (objLeft) {
              stickRotate = autoTurnSpeed * -1.0;
            }
            else {
              stickRotate = autoTurnSpeed;
            }
          }
        }
        else if (atLine) {   //AT LINE
          screenColor(white); //White
          degMove(5, 0, 180, 50); //Spin 180deg ()
          degMove(1, 7, 0, 40); //Move forward 7 in (V=40)
        }
        else if (nearObject && charge > 1) {  //AT OBJECT  + CHARGE
          if (BorderDetector.reflectivity() <= 10) { //Check at line
            screenColor(white);
            braking = true;
          }
          else {
            screenColor(purple); //Purple
            updateVelocity(charge, true); //Set vel
            spinMotors();
          }
        }
        else if (nearObject && charge == 0) {  // AT OBJECT NO CHARGE
          updateVelocity(0, true); //Stop
          braking = true;
        }
      }
      else {  //MANUAL CONTROL
        screenColor(green);
        stickRotate = Controller1.Axis1.position();
        stickForward = Controller1.Axis3.position();
        stickSideways = Controller1.Axis4.position();
      }
      if (!nearObject) {
        updateVelocity(1, false); //Update drive normally if not charging
        spinMotors();
      }
    }
    else if (braking) { //Brake if braking == true
      screenColor(red);
      eBrake();
    }
  }
  wait(5, msec); 
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
  
  updateConsole();
  omniControl();
}