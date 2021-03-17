using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern controller Controller1;
extern motor North1;
extern motor South2;
extern motor West3L;
extern motor East4R;
extern signature Vision13__ORANGECUBE;
extern signature Vision13__PURPLECUBE;
extern signature Vision13__GREENCUBE;
extern signature Vision13__SIG_4;
extern signature Vision13__SIG_5;
extern signature Vision13__SIG_6;
extern signature Vision13__SIG_7;
extern vision Vision13;
extern distance Laser;
extern line BorderDetector;
extern bumper StopButton;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );