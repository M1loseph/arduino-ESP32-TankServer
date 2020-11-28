#include "engines_controller.hpp"
#include "debug.hpp"

#if ENGINE_DEBUG

#define LOG_ENGINE(message) LOG(message)
#define LOG_ENGINE_NL(message) LOG_NL(message)
#define LOG_ENGINE_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_ENGINE(message)
#define LOG_ENGINE_NL(message)
#define LOG_ENGINE_F(...)

#endif // ENGINE_DEBUG

namespace json_parser
{
    engines_controller::engines_controller() : templated_controller("engines")
    {
    }

    void engines_controller::disable_speed_left()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_LEFT, 0);
#else
        digitalWrite(PIN_SPEED_LEFT, LOW);
#endif
    }

    void engines_controller::enable_speed_left()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_LEFT, _speed_left);
#else
        analogWrite(PIN_SPEED_LEFT, _speed_left);
#endif
    }

    void engines_controller::disable_speed_right()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_RIGHT, 0);
#else
        digitalWrite(PIN_SPEED_RIGHT, LOW);
#endif
    }

    void engines_controller::enable_speed_right()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_RIGHT, _speed_right);
#else
        analogWrite(PIN_SPEED_RIGHT, _speed_right);
#endif
    }

    bool engines_controller::initialize()
    {
        pinMode(PIN_FRONT_LEFT, OUTPUT);
        pinMode(PIN_FRONT_RIGHT, OUTPUT);
        pinMode(PIN_BACK_LEFT, OUTPUT);
        pinMode(PIN_BACK_RIGHT, OUTPUT);
#ifdef ESP32
        // two 1000 Hz channels
        ledcSetup(PWM_CHANNEL_LEFT, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcSetup(PWM_CHANNEL_RIGHT, PWM_FREQUENCY, PWM_RESOLUTION);

        ledcAttachPin(PIN_SPEED_LEFT, PWM_CHANNEL_LEFT);
        ledcAttachPin(PIN_SPEED_RIGHT, PWM_CHANNEL_RIGHT);
#else
        pinMode(PIN_SPEED_LEFT, OUTPUT);
        pinMode(PIN_SPEED_RIGHT, OUTPUT);
#endif
        LOG_ENGINE_NL("[engine_controller] initialized engine pins")

        _events.reserve(20U);
        bool if_added = true;

        if_added &= add_event(FORWARD, &engines_controller::forward);
        if_added &= add_event(FORWARD_LEFT, &engines_controller::forward_left);
        if_added &= add_event(FORWARD_RIGHT, &engines_controller::forward_right);

        if_added &= add_event(BACKWARD, &engines_controller::backward);
        if_added &= add_event(BACKWARD_LEFT, &engines_controller::backward_left);
        if_added &= add_event(BACKWARD_RIGHT, &engines_controller::backward_right);

        if_added &= add_event(STOP, &engines_controller::stop);
        if_added &= add_event(STOP_LEFT, &engines_controller::stop_left);
        if_added &= add_event(STOP_RIGHT, &engines_controller::stop_right);

        if_added &= add_event(FASTER, &engines_controller::faster);
        if_added &= add_event(FASTER_LEFT, &engines_controller::faster_left);
        if_added &= add_event(FASTER_RIGHT, &engines_controller::faster_right);

        if_added &= add_event(SLOWER, &engines_controller::slower);
        if_added &= add_event(SLOWER_LEFT, &engines_controller::slower_left);
        if_added &= add_event(SLOWER_RIGHT, &engines_controller::slower_right);

        if_added &= add_event(KEEP_SPEED, &engines_controller::keep_speed);
        if_added &= add_event(KEEP_SPEED_LEFT, &engines_controller::keep_speed_left);
        if_added &= add_event(KEEP_SPEED_RIGHT, &engines_controller::keep_speed_right);

        if_added &= add_event(SPEED, &engines_controller::set_speed);
        if_added &= add_event(SPEED_LEFT, &engines_controller::set_speed_left);
        if_added &= add_event(SPEED_RIGHT, &engines_controller::set_speed_right);

        return true;
    }

    bool engines_controller::forward(const JsonObjectConst *json)
    {
        forward_left(json);
        forward_right(json);
        LOG_ENGINE_NL("[engine_controller] both forward")
        return true;
    }

    bool engines_controller::forward_left(const JsonObjectConst *json)
    {
        // disabling speed pin just in case
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, HIGH);
        digitalWrite(PIN_BACK_LEFT, LOW);
        _direction_left = direction::FORWARD;
        enable_speed_left();
        LOG_ENGINE_NL("[engine_controller] forward left")
        return true;
    }

    bool engines_controller::forward_right(const JsonObjectConst *json)
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, HIGH);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        _direction_right = direction::FORWARD;
        enable_speed_right();
        LOG_ENGINE_NL("[engine_controller] forward right")
        return true;
    }

    bool engines_controller::backward(const JsonObjectConst *json)
    {
        backward_left(json);
        backward_right(json);
        LOG_ENGINE_NL("[engine_controller] both backward")
        return true;
    }

    bool engines_controller::backward_left(const JsonObjectConst *json)
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, HIGH);
        _direction_left = direction::BACKWARD;
        enable_speed_left();
        LOG_ENGINE_NL("[engine_controller] backward left")
        return true;
    }

    bool engines_controller::backward_right(const JsonObjectConst *json)
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, HIGH);
        _direction_right = direction::BACKWARD;
        enable_speed_right();
        LOG_ENGINE_NL("[engine_controller] backward right")
        return true;
    }

    bool engines_controller::stop(const JsonObjectConst *json)
    {
        stop_left(json);
        stop_right(json);
        LOG_ENGINE_NL("[engine_controller] both stop")
        return true;
    }

    bool engines_controller::stop_left(const JsonObjectConst *json)
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, LOW);
        _direction_left = direction::STOP;
        LOG_ENGINE_NL("[engine_controller] stop left")
        return true;
    }

    bool engines_controller::stop_right(const JsonObjectConst *json)
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        _direction_right = direction::STOP;
        LOG_ENGINE_NL("[engine_controller] stop right")
        return true;
    }

    bool engines_controller::rotate_left(const JsonObjectConst *json)
    {
        backward_left(json);
        forward_right(json);
        LOG_ENGINE_NL("[engine_controller] rotate left")
        return true;
    }

    bool engines_controller::rotate_right(const JsonObjectConst *json)
    {
        forward_left(json);
        backward_right(json);
        LOG_ENGINE_NL("[engine_controller] rotate right")
        return true;
    }

    bool engines_controller::slower(const JsonObjectConst *json)
    {
        slower_left(json);
        slower_right(json);
        LOG_ENGINE_NL("[engine_controller] both slower")
        return true;
    }

    bool engines_controller::slower_left(const JsonObjectConst *json)
    {
        _speed_controll_left = speed_controll::SLOWER;
        LOG_ENGINE_NL("[engine_controller] slower left")
        return true;
    }

    bool engines_controller::slower_right(const JsonObjectConst *json)
    {
        _speed_controll_right = speed_controll::SLOWER;
        LOG_ENGINE_NL("[engine_controller] slower right")
        return true;
    }

    bool engines_controller::faster(const JsonObjectConst *json)
    {
        faster_left(json);
        faster_right(json);
        LOG_ENGINE_NL("[engine_controller] both faster")
        return true;
    }

    bool engines_controller::faster_left(const JsonObjectConst *json)
    {
        _speed_controll_left = speed_controll::FASTER;
        LOG_ENGINE_NL("[engine_controller] faster left")
        return true;
    }

    bool engines_controller::faster_right(const JsonObjectConst *json)
    {
        _speed_controll_right = speed_controll::FASTER;
        LOG_ENGINE_NL("[engine_controller] faster right")
        return true;
    }

    bool engines_controller::keep_speed(const JsonObjectConst *json)
    {
        keep_speed_left(json);
        keep_speed_right(json);
        LOG_ENGINE_NL("[engine_controller] both keep speed")
        return true;
    }

    bool engines_controller::keep_speed_left(const JsonObjectConst *json)
    {
        _speed_controll_left = speed_controll::KEEP_SPEED;
        LOG_ENGINE_NL("[engine_controller] left keeps speed")
        return true;
    }

    bool engines_controller::keep_speed_right(const JsonObjectConst *json)
    {
        _speed_controll_right = speed_controll::KEEP_SPEED;
        LOG_ENGINE_NL("[engine_controller] right keeps speed")
        return true;
    }

    size_t engines_controller::get_speed_from_json(const JsonObjectConst *json, bool *succ)
    {
        if (json)
        {
            if (json->containsKey(SPEED_KEY))
            {
                uint32_t new_speed = (*json)[SPEED_KEY];
                if (new_speed <= SPEED_MAX)
                {
                    (*succ) = true;
                    LOG_ENGINE_F("[engine_controller] got speed %d\n", new_speed)
                    return new_speed;
                }
                else
                {
                    LOG_ENGINE_F("[engine_controller] too fast %d\n", new_speed)
                }
            }
            else
            {
                LOG_ENGINE_NL("[engine_controller] no speed key")
            }
        }
        else
        {
            LOG_ENGINE_NL("[engine_controller] no json")
        }
        (*succ) = false;
        return 0U;
    }

    bool engines_controller::set_speed(const JsonObjectConst *json)
    {
        bool succ = false;
        size_t new_speed = get_speed_from_json(json, &succ);
        if (succ)
        {
            _speed_left = new_speed;
            _speed_right = new_speed;
        }
        return succ;
    }

    bool engines_controller::set_speed_left(const JsonObjectConst *json)
    {
        bool succ = false;
        size_t new_speed = get_speed_from_json(json, &succ);
        if (succ)
        {
            _speed_left = new_speed;
        }
        return succ;
    }

    bool engines_controller::set_speed_right(const JsonObjectConst *json)
    {
        bool succ = false;
        size_t new_speed = get_speed_from_json(json, &succ);
        if (succ)
        {
            _speed_right = new_speed;
        }
        return succ;
    }

    void engines_controller::update()
    {
        static unsigned long last_update = millis();
        if (millis() - last_update > SPEED_CHANGE_INTERVAL)
        {
            bool left_speed_changed = false;
            bool right_speed_changed = false;

            if (_speed_controll_left == speed_controll::SLOWER && _speed_left > 0U)
            {
                _speed_left--;
                left_speed_changed = true;
            }
            else if (_speed_controll_left == speed_controll::FASTER && _speed_left < SPEED_MAX)
            {
                _speed_left++;
                left_speed_changed = true;
            }

            if (_speed_controll_right == speed_controll::SLOWER && _speed_right > 0U)
            {
                _speed_right--;
                right_speed_changed = true;
            }
            else if (_speed_controll_right == speed_controll::FASTER && _speed_right < SPEED_MAX)
            {
                _speed_right++;
                right_speed_changed = true;
            }

            if (left_speed_changed)
            {
                LOG_ENGINE_F("[engine_controller] new left speed: %d\n", _speed_left)
            }
            if (right_speed_changed)
            {
                LOG_ENGINE_F("[engine_controller] new right speed: %d\n", _speed_right)
            }

            last_update = millis();
        }
    }
} // namespace json_parser