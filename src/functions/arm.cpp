#include "arm.h"
#include "debug.h"
#include "global_parser.h"

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
    const char *SERVO_MINUS = "SERVO_MINUS";
    const char *SERVO_PLUS = "SERVO_PLUS";
    const char *SERVO_STOP = "SERVO_STOP";
    const char *SERVO_ANGLE = "SERVO_ANGLE";
  } // namespace commands

  constexpr uint32_t PULSE_MS_MIN = 600U;
  constexpr uint32_t PULSE_MS_MAX = 2500U;
  constexpr uint8_t PULSES_FREQUENCY = 50U;

  constexpr uint32_t SERVO_TIMEOUT = 20U;

  Adafruit_PWMServoDriver pwm;

  servo_data base{5, 165, 90, 90, 0};
  servo_data shoulder{40, 150, 90, 90, 3};
  servo_data elbow{0, 130, 90, 90, 7};
  servo_data wrist{70, 180, 90, 90, 8};
  servo_data rotation{0, 180, 90, 90, 12};
  servo_data claw{60, 115, 80, 80, 11};

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

  // assumes servo is at 1st position
  static servo_data *get_servo_from_buffer(const CommandBuffer &b)
  {
    auto arm_index = b.int_at(1);
    if (arm_index.success && arm_index.i >= 0 && arm_index.i < ARRAY_LENGTH(arm))
      return arm[arm_index.i];

    return nullptr;
  }

  // ================
  // EXPORT
  // ================

  void init_arm()
  {
    Wire.begin();
    pwm.begin();
    pwm.setOscillatorFrequency(27000000U);
    pwm.setPWMFreq(PULSES_FREQUENCY); // Analog servos run at ~50 Hz updates
    for (const auto &servo : arm)
      send_angle(*servo);
  }

  void servo_minus(const CommandBuffer &b)
  {
    servo_data *servo = get_servo_from_buffer(b);
    if (servo)
    {
      servo->destination_angle = servo->MIN_ANGLE;
      LOG_ARM_F("Servo at index %d moving to %d\n", servo->extern_module_pin, servo->destination_angle);
    }
    else
      LOG_ARM_NL("Wrong servo index");
  }

  void servo_plus(const CommandBuffer &b)
  {
    servo_data *servo = get_servo_from_buffer(b);
    if (servo)
    {
      servo->destination_angle = servo->MAX_ANGLE;
      LOG_ARM_F("Servo at index %d moving to %d\n", servo->extern_module_pin, servo->destination_angle);
    }
    else
      LOG_ARM_NL("Wrong servo index");
  }

  void servo_stop(const CommandBuffer &b)
  {
    servo_data *servo = get_servo_from_buffer(b);
    if (servo)
    {
      servo->destination_angle = servo->current_angle;
      LOG_ARM_F("Servo at index %d stopping at angle %d\n", servo->extern_module_pin, servo->current_angle);
    }
    else
      LOG_ARM_NL("Wrong servo index");
  }

  void servo_angle(const CommandBuffer &b)
  {
    servo_data *servo = get_servo_from_buffer(b);
    if (servo)
    {
      auto new_angle = b.int_at(2);
      // now let's check if angle is in bounds
      if (new_angle.success && new_angle.i >= servo->MIN_ANGLE && new_angle.i <= servo->MAX_ANGLE)
        servo->destination_angle = new_angle.i;
      else
        LOG_ARM_F("Angle error - success: %d, angle: %d, servo: %d\n", new_angle.success, new_angle.i, servo->extern_module_pin);
    }
    else
      LOG_ARM_NL("Wrong servo index");
  }

  void update_servos_movement()
  {
    static unsigned long timer = millis();
    if (millis() - timer > SERVO_TIMEOUT)
    {
      while (xSemaphoreTake(global_parser::semaphore, (TickType_t)10) != pdTRUE)
        ;
      for (auto &servo : arm)
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
      xSemaphoreGive(global_parser::semaphore);
      timer = millis();
    }
  }

} // namespace arm