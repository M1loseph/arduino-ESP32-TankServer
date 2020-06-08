#ifndef __COMMANDS_H__
#define __COMMANDS_H__

namespace Command
{
  namespace Mcu
  {
    const char *TANK_FORWARD = "FORWARD";
    const char *TANK_BACKWARD = "BACKWARD";
    const char *TANK_LEFT = "LEFT";
    const char *TANK_RIGHT = "RIGHT";
    const char *TANK_STOP = "STOP";
    const char *TANK_SPEED = "SPEED";
    const char *TURRET_FORWARD = "TFORWARD";
    const char *TURRET_BACKWARD = "TBACKWARD";
    const char *TURRET_STOP = "TSTOP";
    const char *GUN_MOVE = "GMOVE";
    const char *GUN_STOP = "GSTOP";
    const char *MOVE = "MOVE";
  } // namespace MCU

  namespace Mega
  {
    const char *LOST_CONNECTION = "LC";
    const char *BASE = "BASE";
    const char *SHOULDER = "SHOULDER";
    const char *ELBOW_1 = "ELBOW1";
    const char *ELBOW_2 = "ELBOW2";
    const char *WRIST = "WRIST";
    const char *CLAW = "CLAW";
  }
} // namespace Command

#endif //__COMMANDS_H__