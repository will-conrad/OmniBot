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
/*vex-vision-config:begin*/
signature Vision13__ORANGECUBE = signature (1, 4825, 9985, 7406, -3269, -2649, -2960, 1.2, 0);
signature Vision13__PURPLECUBE = signature (2, 1773, 2657, 2216, 9411, 12073, 10742, 3.8, 0);
signature Vision13__GREENCUBE = signature (3, -7489, -6181, -6836, -3821, -2819, -3320, 1.6, 0);
vision Vision13 = vision (PORT13, 34, Vision13__ORANGECUBE, Vision13__PURPLECUBE, Vision13__GREENCUBE);
/*vex-vision-config:end*/
distance Laser = distance(PORT10);
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