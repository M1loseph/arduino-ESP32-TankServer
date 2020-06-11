#ifndef __COMMANDS_H__
#define __COMMANDS_H__

namespace Command
{
  namespace Mcu
  {
    const char *TANK_FORWARD_L = "FORWARDL";
    const char *TANK_FORWARD_R = "FORWARDR";

    const char *TANK_BACKWARD_L = "BACKWARDL";
    const char *TANK_BACKWARD_R = "BACKWARDR";

    const char *TANK_STOP_L = "STOPL";
    const char *TANK_STOP_R = "STOPR";
    
    const char *TANK_SPEED = "SPEED";

    const char *MOVE = "MOVE";
  } // namespace Mcu

  namespace Nano
  {
    const char *LOST_CONNECTION = "LC";
    const char *BASE = "BASE";
    const char *SHOULDER = "SHOULDER";
    const char *ELBOW_1 = "ELBOW1";
    const char *ELBOW_2 = "ELBOW2";
    const char *WRIST = "WRIST";
    const char *CLAW = "CLAW";
    const char *MP3_COMMAND = "MP3";
  } // namespace Nano
} // namespace Command

#endif //__COMMANDS_H__