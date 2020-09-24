#include "arm.h"

#if ARM_DEBUG

#define LOG_ARM(message) LOG(message)
#define LOG_ARM(message) LOG_NL(message)

#else

#define LOG_ARM(message)
#define LOG_ARM(message)

#endif // ARM_DEBUG

namespace arm
{
  namespace commands
  {
    const char *BASE = "BASE";
    const char *SHOULDER = "SHOULDER";
    const char *ELBOW = "ELBOW";
    const char *WRIST = "WRIST";
    const char *ROTATION = "ROTATION";
    const char *CLAW = "CLAW";

    namespace modifiers
    {
      const char *MINUS = "MINUS";
      const char *PLUS = "PLUS";
      const char *STOP = "STOP";
      const char *SET_ANGLE = "SET_ANGLE";
    } // namespace modifiers
  }   // namespace commands

  // ================
  // VARIABLES
  // ================

  constexpr size_t PULSE_MS_MIN = 600;
  constexpr size_t PULSE_MS_MAX = 2500;
  constexpr uchar PULSES_FREQUENCY = 50;

  constexpr size_t SERVO_TIMEOUT = 20;

  Adafruit_PWMServoDriver pwm;

  servo_data base{5, 165, 90, 0, servo_direction::STOP};
  servo_data shoulder{40, 150, 90, 1, servo_direction::STOP};
  servo_data elbow{0, 130, 90, 2, servo_direction::STOP};
  servo_data wrist{70, 180, 90, 3, servo_direction::STOP};
  servo_data rotation{0, 180, 90, 4, servo_direction::STOP};
  servo_data claw{60, 115, 80, 5, servo_direction::STOP};

  // constexpr uchar MIN_ANGLES[] = {5, 40, 0, 70, 0, 60};
  // constexpr uchar MAX_ANGLES[] = {165, 150, 130, 180, 180, 115};
  // constexpr uchar DEF_ANGLES[] = {90, 90, 90, 90, 90, 80};

  // int directions[SERVOS] = {SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP};

  // ================
  // PRIVATE
  // ================

  static void update_servo(servo_data &servo, const CommandBuffer &b)
  {
    const char *command = b.word_at(1);
    if (command)
    {
      if(strcmp(command, commands::modifiers::MINUS) == 0)
        servo.direction = servo_direction::MINUS;
      else if(strcmp(command, commands::modifiers::PLUS) == 0)
        servo.direction = servo_direction::PLUS;
      else if(strcmp(command, commands::modifiers::STOP) == 0)
        servo.direction = servo_direction::STOP;
      else if(strcmp(command, commands::modifiers::SET_ANGLE))
      {

      }
    }
    
  }

  // ================
  // EXPORT
  // ================

  void init_arm()
  {
    Wire.begin();
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(PULSES_FREQUENCY); // Analog servos run at ~50 Hz updates
  }

  void move_base(const CommandBuffer &b)
  {
    update_servo(base, b);
  }

  void move_shoulder(const CommandBuffer &b)
  {
    update_servo(shoulder, b);
  }

  void move_elbow(const CommandBuffer &b)
  {
    update_servo(elbow, b);
  }

  void move_wrist(const CommandBuffer &b)
  {
    update_servo(wrist, b);
  }

  void move_rotation(const CommandBuffer &b)
  {
    update_servo(rotation, b);
  }

  void move_claw(const CommandBuffer &b)
  {
    update_servo(claw, b);
  }

  // makes it easier to use next function
  servo_data *helper_table[] = {
      &base,
      &shoulder,
      &elbow,
      &wrist,
      &rotation,
      &claw};

  void update_servos_movement()
  {
    static unsigned long timer = millis();
    if (millis() - timer > SERVO_TIMEOUT)
    {
      for (uchar i = 0; i < sizeof(helper_table) / sizeof(servo_data *); i++)
      {
        servo_data &servo = *(helper_table[i]);

        bool send_changes = false;
        if (servo.direction == servo_direction::PLUS && servo.current_angle < servo.MAX_ANGLE)
        {
          servo.current_angle++;
          send_changes = true;
        }
        else if (servo.direction == servo_direction::MINUS && servo.current_angle > servo.MIN_ANGLE)
        {
          servo.current_angle--;
          send_changes = true;
        }
        if (send_changes)
        {
          uint16_t pulse = static_cast<uint16_t>(map(servo.current_angle, 0, 180, PULSE_MS_MIN, PULSE_MS_MAX));
          pwm.writeMicroseconds(i, pulse);
        }
      }
      timer = millis();
    }
  }

} // namespace arm