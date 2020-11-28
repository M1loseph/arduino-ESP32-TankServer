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

        bool if_added = true;

        if_added &= add_event(FORWARD, &engines_controller::forward);
        if_added &= add_event(BACKWARD, &engines_controller::backward);
        if_added &= add_event(STOP, &engines_controller::stop);
        if_added &= add_event(FASTER, &engines_controller::faster);
        if_added &= add_event(SLOWER, &engines_controller::slower);
        if_added &= add_event(KEEP_SPEED, &engines_controller::keep_speed);
        if_added &= add_event(SPEED, &engines_controller::set_speed);

        return true;
    }

    bool engines_controller::forward(const JsonObjectConst *json)
    {
        bool if_executed = false;
        const char *engines = get_engine_from_json(json);
        if (engines)
        {
            if (!strcmp(engines, BOTH))
            {
                forward_left();
                forward_right();
                if_executed = true;
            }
            else if (!strcmp(engines, LEFT))
            {
                forward_left();
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                forward_right();
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::forward_left()
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, HIGH);
        digitalWrite(PIN_BACK_LEFT, LOW);
        _direction_left = direction::FORWARD;
        enable_speed_left();
        LOG_ENGINE_F("[%s] forward left\n", _name)
    }

    void engines_controller::forward_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, HIGH);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        _direction_right = direction::FORWARD;
        enable_speed_right();
        LOG_ENGINE_F("[%s] forward right\n", _name)
    }

    bool engines_controller::backward(const JsonObjectConst *json)
    {
        bool if_executed = false;
        const char *engines = get_engine_from_json(json);
        if (engines)
        {
            if (!strcmp(engines, BOTH))
            {
                backward_left();
                backward_right();
                if_executed = true;
            }
            else if (!strcmp(engines, LEFT))
            {
                backward_left();
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                backward_right();
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::backward_left()
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, HIGH);
        _direction_left = direction::BACKWARD;
        enable_speed_left();
        LOG_ENGINE_F("[%s] backward left\n", _name)
    }

    void engines_controller::backward_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, HIGH);
        _direction_right = direction::BACKWARD;
        enable_speed_right();
        LOG_ENGINE_NL("[engine_controller] backward right")
    }

    bool engines_controller::stop(const JsonObjectConst *json)
    {
        bool if_executed = false;
        const char *engines = get_engine_from_json(json);
        if (engines)
        {
            if (!strcmp(engines, BOTH))
            {
                stop_left();
                stop_right();
                if_executed = true;
            }
            else if (!strcmp(engines, LEFT))
            {
                stop_left();
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                stop_right();
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::stop_left()
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, LOW);
        _direction_left = direction::STOP;
        LOG_ENGINE_F("[%s] stop left\n", _name)
    }

    void engines_controller::stop_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        _direction_right = direction::STOP;
        LOG_ENGINE_F("[%s] stop right\n", _name)
    }

    bool engines_controller::rotate(const JsonObjectConst *json)
    {
        bool if_executed = false;
        const char *engines = get_engine_from_json(json);
        if (engines)
        {
            if (!strcmp(engines, LEFT))
            {
                rotate_left();
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                rotate_right();
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::rotate_left()
    {
        backward_left();
        forward_right();
        LOG_ENGINE_F("[%s] rotate left\n", _name)
    }

    void engines_controller::rotate_right()
    {
        forward_left();
        backward_right();
        LOG_ENGINE_F("[%s] rotate right\n", _name)
    }

    bool engines_controller::slower(const JsonObjectConst *json)
    {
        bool if_executed = false;
        const char *engines = get_engine_from_json(json);
        if (engines)
        {
            if (!strcmp(engines, BOTH))
            {
                slower_left();
                slower_right();
                if_executed = true;
            }
            else if (!strcmp(engines, LEFT))
            {
                slower_left();
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                slower_right();
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::slower_left()
    {
        _speed_controll_left = speed_controll::SLOWER;
        LOG_ENGINE_F("[%s] slower left\n", _name)
    }

    void engines_controller::slower_right()
    {
        _speed_controll_right = speed_controll::SLOWER;
        LOG_ENGINE_F("[%s] slower right\n", _name)
    }

    bool engines_controller::faster(const JsonObjectConst *json)
    {
        bool if_executed = false;
        const char *engines = get_engine_from_json(json);
        if (engines)
        {
            if (!strcmp(engines, BOTH))
            {
                faster_left();
                faster_right();
                if_executed = true;
            }
            else if (!strcmp(engines, LEFT))
            {
                faster_left();
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                faster_right();
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::faster_left()
    {
        _speed_controll_left = speed_controll::FASTER;
        LOG_ENGINE_F("[%s] faster left\n", _name)
    }

    void engines_controller::faster_right()
    {
        _speed_controll_right = speed_controll::FASTER;
        LOG_ENGINE_F("[%s] faster right\n", _name)
    }

    bool engines_controller::keep_speed(const JsonObjectConst *json)
    {
        bool if_executed = false;
        const char *engines = get_engine_from_json(json);
        if (engines)
        {
            if (!strcmp(engines, BOTH))
            {
                keep_speed_left();
                keep_speed_right();
                if_executed = true;
            }
            else if (!strcmp(engines, LEFT))
            {
                keep_speed_left();
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                keep_speed_right();
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::keep_speed_left()
    {
        _speed_controll_left = speed_controll::KEEP_SPEED;
        LOG_ENGINE_F("[%s] left keeps speed\n", _name)
    }

    void engines_controller::keep_speed_right()
    {
        _speed_controll_right = speed_controll::KEEP_SPEED;
        LOG_ENGINE_F("[%s] right keeps speed\n", _name)
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
        bool if_executed = false;
        bool succ = false;
        size_t new_speed = get_speed_from_json(json, &succ);
        const char *engines = get_engine_from_json(json);
        if (succ && engines)
        {
            if (!strcmp(engines, BOTH))
            {
                set_speed_left(new_speed);
                set_speed_right(new_speed);
                if_executed = true;
            }
            else if (!strcmp(engines, LEFT))
            {
                set_speed_left(new_speed);
                if_executed = true;
            }
            else if (!strcmp(engines, RIGHT))
            {
                set_speed_right(new_speed);
                if_executed = true;
            }
        }
        return if_executed;
    }

    void engines_controller::set_speed_left(uint32_t new_speed)
    {
        _speed_left = new_speed;
    }

    void engines_controller::set_speed_right(uint32_t new_speed)
    {
        _speed_right = new_speed;
    }

    const char *engines_controller::get_engine_from_json(const JsonObjectConst *json)
    {
        if (json)
        {
            if (json->containsKey(ENGINE_KEY))
            {
                return (*json)[ENGINE_KEY];
            }
            else
            {
                LOG_ENGINE_NL("[engine_controller] no engine key")
            }
        }
        else
        {
            LOG_ENGINE_NL("[engine_controller] no json")
        }
        return nullptr;
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