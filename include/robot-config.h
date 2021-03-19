using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor North1;
extern motor South2;
extern motor West3L;
extern motor East4R;
extern distance LaserL;
extern line BorderDetector;
extern bumper StopButton;
extern distance LaserR;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );