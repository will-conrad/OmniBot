// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// North1               motor         1               
// South2               motor         2               
// West3L               motor         3               
// East4R               motor         4               
// Vision13             vision        13              
// Laser                distance      10              
// BorderDetector       line          B               
// StopButton           bumper        C               
// ---- END VEXCODE CONFIGURED DEVICES ----
#include<vex.h>
#include<iostream>

using namespace vex;
THIS IS A BIG CHANGE!!!!
// ---- INIT VISION SIGNATURES ---- //
vision::signature ORANGECUBE (1, 4825, 9985, 7406, -3269, -2649, -2960, 1.200, 0);
vision::signature PURPLECUBE (2, 1773, 2657, 2216, 9411, 12073, 10742, 3.800, 0);
vision::signature GREENCUBE (3, -7489, -6181, -6836, -3821, -2819, -3320, 1.600, 0);
vision::signature SIG_4 (4, 0, 0, 0, 0, 0, 0, 2.500, 0);
vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 2.500, 0);
vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 2.500, 0);
vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 2.500, 0);
vex::vision vision1 ( vex::PORT1, 34, ORANGECUBE, PURPLECUBE, GREENCUBE, SIG_4, SIG_5, SIG_6, SIG_7 );
// ---- VARIABLES ---- //
void checkLine();
void spinMotors(); 
void halt();
void eBrake();
void unbrake();
void updateVelocity(int n);

int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, Vision13_objectIndex = 0;
float northV, southV, eastV, westV, stickRotate, stickForward, stickSideways, objectLoc, autoTurnSpeed, autoFollowSpeed, autoSensitivity, charge;
bool useController, autonomous, running, objectExists, objLeft, braking, atLine, atObject, seeObject, brakeMem, autoMem, lineMem, objMem;

// ---- FUNCTIONS ---- //
void spinMotors() {
  // Spin wheels to preset velocity
  North1.spin(forward);
  South2.spin(forward);
  West3L.spin(forward);
  East4R.spin(forward);
}
void updateVelocity(int n) {
  //Calculate correct wheel speeds
  northV = (stickForward + stickSideways) + stickRotate;
  eastV = (stickForward - stickSideways) - stickRotate;
  westV = (stickForward - stickSideways) + stickRotate;
  southV = (stickForward + stickSideways) - stickRotate;
  //Set velocity
  if (n == 1) {
    North1.setVelocity(northV, percent);
    South2.setVelocity(southV, percent);
    West3L.setVelocity(westV, percent);
    East4R.setVelocity(eastV, percent);
    spinMotors();
  }
  else if (n > 1) {
    North1.setVelocity(n, percent);
    South2.setVelocity(n, percent);
    West3L.setVelocity(n, percent);
    East4R.setVelocity(n, percent);
    spinMotors();
  }
  else {
    North1.stop();
    South2.stop();
    West3L.stop();
    East4R.stop();
  }
}
void halt() {
  //Kill leftover sticks
  stickRotate = 0.0;
  stickForward = 0.0;
  stickSideways = 0.0;
  //Kill leftover velocity
  /* updateVelocity() does this
  northV = 0.0;
  southV = 0.0;
  westV = 0.0;
  eastV = 0.0;
  */
  //Update
  updateVelocity(0);
}
void eBrake() {
  //Disable motors
  //running = false;
  //Disable controller input
  //useController = false;
  //Set motor stopping behavior
  North1.setStopping(hold);
  South2.setStopping(hold);
  West3L.setStopping(hold);
  East4R.setStopping(hold);
  //Kill input and velocity
  halt();
}
void unbrake() {
  //Engage motors
  running = true;
  //Engage controller input
  //useController = true;
  //Reset motor stop behavior
  North1.setStopping(coast);
  South2.setStopping(coast);
  West3L.setStopping(coast);
  East4R.setStopping(coast);
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
  if (Laser.objectDistance(mm) < 127.0 && Laser.objectDistance(mm) > 0.1) {
    atObject = true;
  }
  else {
    atObject = false;
  }
}
void updateConsole() {
  if (braking != brakeMem || autonomous != autoMem || atLine != lineMem || atObject != objMem) {
    Brain.Screen.clearScreen(); //Clear Text
    if (braking) {
      Brain.Screen.print("Braking: True\n");
    }
    else {
      Brain.Screen.print("Braking: False\n");
    }
    if (autonomous) {
      Brain.Screen.print("Autonomous: True\n");
    }
    else {
      Brain.Screen.print("Autonomous: False\n");
    }
    if (atLine) {
      Brain.Screen.print("At Line: True\n");
    }
    else {
      Brain.Screen.print("At Line: False\n");
    }
    if (atObject) {
      Brain.Screen.print("At Object: True\n");
    }
    else {
      Brain.Screen.print("At Object: False\n");
    }

    brakeMem = braking;
    autoMem = autonomous;
    lineMem = atLine;
    objMem = atObject;
  }
}
void turnaround() {
  halt();
  updateVelocity(50);
  float wheelDeg = 645.0;
  North1.spinFor(wheelDeg, degrees, false);
  South2.spinFor(wheelDeg * -1, degrees, false);
  East4R.spinFor(wheelDeg * -1, degrees, false);
  West3L.spinFor(wheelDeg, degrees, true);
}
void screenColor(color c) {
  Brain.Screen.setFillColor(c);
  Brain.Screen.drawRectangle(0, 0, 500, 500);
  updateConsole();
}
// ===========================================================================================================


// ---- MAIN DRIVE CONTROL ---- //
int omniControl() {
  //Autonomous speeds
  autoFollowSpeed = 20.0;
  autoTurnSpeed   = 20.0;
  autoSensitivity = 4;   //Lower = more jumpy (Overcorrects)
  charge = 50;
  running = true;
  atLine = false;
  atObject = false;
  //useController = true;
  
  autonomous = false;
  //Forever
  while (true) {
    
    //updateConsole();
    checkLine(); 
    checkObject();
    
    

    if (StopButton.pressing()) {
      braking = true;
      autonomous = false;
      Controller1.rumble(rumbleShort);
      
    }
    if (running && !braking) {
      
      if (autonomous) { //If bot is in auto mode
        
        if (!atLine && !atObject) { //NOT at line AND NOT at Object
          screenColor(orange); //Orange if autonomous
          
          Vision13.takeSnapshot(ORANGECUBE);
          Vision13.takeSnapshot(GREENCUBE); 
          Vision13.takeSnapshot(PURPLECUBE);

          if (Vision13.objectCount > 0 && Vision13.objectCount < 3) { //Sees object
            seeObject = true;
            objectLoc = ((Vision13.objects[Vision13_objectIndex].centerX - 157.5) / autoSensitivity) * -1.0; //Set relative object position
            stickRotate = objectLoc;
            
            if (objectLoc > 0.0) { //Determine where object is
              objLeft = false;
            }
            else {
              objLeft = true;
            }
            
            stickForward = autoFollowSpeed; //Go forward
            
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
          //turnaround();
          if (true) {
            Controller1.rumble(rumbleShort);
          

            atLine = false;
          }
        }
        else if (atObject && charge > 1) {  //AT OBJECT  + CHARGE
          screenColor(purple); //Purple
          updateVelocity(charge); //Set vel
          if (BorderDetector.reflectivity() <= 10) {
            screenColor(cyan);
            braking = true;
          }
        }
        else if (atObject && charge == 0) {  // AT OBJECT
          halt();
          braking = true;
        }
      }
      else {  //MANUAL CONTROL
        screenColor(green);
        stickRotate = Controller1.Axis1.position();
        stickForward = Controller1.Axis3.position();
        stickSideways = Controller1.Axis4.position();
        /*
        if (useController) { useController might be redundant 
          // Continually set joystick inputs to variables so they can be changed by autonomy code later
          screenColor(green);
          stickRotate = Controller1.Axis1.position();
          stickForward = Controller1.Axis3.position();
          stickSideways = Controller1.Axis4.position();
        }
        */
      }
      if (!atObject) {
        updateVelocity(1);
      }
      
      
    }
    else if (!running || braking) {
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
  
  omniControl();
}