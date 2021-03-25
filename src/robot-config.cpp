#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor North1 = motor(PORT1, ratio18_1, false);
motor South2 = motor(PORT2, ratio18_1, true);
motor West3L = motor(PORT3, ratio18_1, false);
motor East4R = motor(PORT4, ratio18_1, true);
distance LaserL = distance(PORT10);
distance LaserR = distance(PORT11);
line BorderDetector = line(Brain.ThreeWirePort.B);
bumper StopButton = bumper(Brain.ThreeWirePort.C);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}