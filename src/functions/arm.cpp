#include "arm.h"
#include "debug.h"

#if ARM_DEBUG

#define LOG_ARM(message) LOG(message)
#define LOG_ARM_NL(message) LOG_NL(message)
#define LOG_ARM_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_ARM(message)
#define LOG_ARM_NL(message)
#define LOG_ARM_F(...)

#endif // ARM_DEBUG

namespace arm
{
  // ================
  // VARIABLES
  // ================

  namespace commands
  {
    const char *SERVO_MINUS = "ARM_MINUS";
    const char *SERVO_PLUS = "ARM_PLUS";
    const char *SERVO_STOP = "ARM_STOP";
    const char *SERVO_ANGLE = "ARM_ANGLE";
  } // namespace commands

  constexpr size_t PULSE_MS_MIN = 600;
  constexpr size_t PULSE_MS_MAX = 2500;
  constexpr uchar PULSES_FREQUENCY = 50;

  constexpr size_t SERVO_TIMEOUT = 20;

  Adafruit_PWMServoDriver pwm;

  servo_data base{5, 165, 90, 0, 90};
  servo_data shoulder{40, 150, 90, 1, 90};
  servo_data elbow{0, 130, 90, 2, 90};
  servo_data wrist{70, 180, 90, 3, 90};
  servo_data rotation{0, 180, 90, 4, 90};
  servo_data claw{60, 115, 80, 5, 80};

  // makes it easier to use some functions
  servo_data *arm[] = {
      &base,
      &shoulder,
      &elbow,
      &wrist,
      &rotation,
      &claw,
  };

#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(a[0]))

  // ================
  // PRIVATE
  // ================

  static void send_angle(const servo_data &servo)
  {
    uint16_t pulse = static_cast<uint16_t>(map(servo.current_angle, 0, 180, PULSE_MS_MIN, PULSE_MS_MAX));
    LOG_ARM_F("Sending pulse %d to servo %d\n", pulse, servo.extern_module_pin);
    pwm.writeMicroseconds(servo.extern_module_pin, pulse);
  }

  // static void set_angle(servo_data &servo, const CommandBuffer &b)
  // {
  //   auto new_angle = b.int_at(2);
  //   if (new_angle.success && new_angle.i >= servo.MIN_ANGLE && new_angle.i <= servo.MAX_ANGLE)
  //   {
  //     servo.current_angle = new_angle.i;
  //     send_angle(servo);
  //   }
  //   else
  //   {
  //     LOG_ARM_F("Angle error - success: %d, angle: %d, servo: %d", new_angle.success, new_angle.i, servo.extern_module_pin);
  //   }
  // }

  // ================
  // EXPORT
  // ================

  void init_arm()
  {
    Wire.begin();
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(PULSES_FREQUENCY); // Analog servos run at ~50 Hz updates
    for (const auto &servo : arm)
      send_angle(*servo);
  }

  void arm_minus(const CommandBuffer &b)
  {
  }

  void arm_plus(const CommandBuffer &b)
  {
  }

  void arm_stop(const CommandBuffer &b)
  {
  }

  void arm_angle(const CommandBuffer &b)
  {
    auto arm_index = b.int_at(1);
    if (arm_index.success && arm_index.i >= 0 && arm_index.i < ARRAY_LENGTH(arm))
    {
      servo_data &servo = *(arm[arm_index.i]);
      auto new_angle = b.int_at(2);
      // now let's check if angle is in bounds
      if (new_angle.success && new_angle.i >= servo.MIN_ANGLE && new_angle.i <= servo.MAX_ANGLE)
      {
        servo.current_angle = new_angle.i;
        send_angle(servo);
      }
      else
      {
        LOG_ARM_F("Angle error - success: %d, angle: %d, servo: %d", new_angle.success, new_angle.i, arm_index.i);
      }
    }
    else
    {
      LOG_ARM_F("Wrong servo index - success: %d, index: %d", arm_index.success, arm_index.i);
    }
  }

  void update_servos_movement()
  {
    static unsigned long timer = millis();
    if (millis() - timer > SERVO_TIMEOUT)
    {
      for (const auto &servo : arm)
      {
        bool send_changes = false;
        if (servo->destination_angle > servo->current_angle && servo->current_angle < servo->MAX_ANGLE)
        {
          servo->current_angle++;
          send_changes = true;
        }
        else if (servo->destination_angle < servo->current_angle && servo->current_angle > servo->MIN_ANGLE)
        {
          servo->current_angle--;
          send_changes = true;
        }
        if (send_changes)
          send_angle(*servo);
      }
      timer = millis();
    }
  }

} // namespace arm