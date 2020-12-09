#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__
#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <functional>
#include "controller.hpp"

namespace json_parser
{
    template<typename T>
    class templated_controller : public controller
    {
    public:
        typedef bool (T::*event)(const JsonObject *);

        typedef struct event_data
        {
            event_data(const char *command, event fun, unsigned long last_update, size_t interval) : command(command),
                                                                                                     fun(fun),
                                                                                                     last_update(last_update),
                                                                                                     interval(interval)
            {
            }

            const char *command;
            event fun;
            unsigned long last_update;
            size_t interval;
        } event_data;

        templated_controller(const char *name, uint32_t json_size) : controller(name, json_size) {}
        virtual ~templated_controller() = default;

        bool add_event(const char *command, event function, size_t interval = IDLE_INTERVAL)
        {
            if (command && !get_event(command) && function)
            {
                unsigned long curr_time = millis();
                _events.push_back(event_data(command, function, curr_time, interval));
                return true;
            }
            else
                return false;
        }

        bool set_interval(const char *command, size_t interval)
        {
            // check if command is valid
            if (command)
            {
                auto *event = get_event(command);
                if (event)
                {
                    event->interval = interval;
                    return true;
                }
            }
            return false;
        }

        handle_resoult try_handle(const JsonObject& json)
        {
            if (can_handle(json))
            {
                return handle(json) ? handle_resoult::ok : handle_resoult::error;
            }
            return handle_resoult::not_permited;
        }

        event_data *get_event(const char *command)
        {
            if (command)
            {
                for (size_t i = 0; i < _events.size(); i++)
                {
                    if (strcmp(command, _events[i].command) == 0)
                    {
                        return &(_events[i]);
                    }
                }
            }
            return nullptr;
        }

    protected:
        bool can_handle(const JsonObject& json) const
        {
            if (json.containsKey(CONTROLLER_KEY))
            {
                const char *controller = json[CONTROLLER_KEY];
                return !strcmp(controller, _name);
            }
            else
                return false;
        }
        std::vector<event_data> _events;

    private:
        bool handle(const JsonObject& json) override
        {
            if (json.containsKey(COMMAND_KEY))
            {
                const char *command = json[COMMAND_KEY];
                for (auto &action : _events)
                {
                    if (!strcmp(action.command, command))
                    {
                        return (*static_cast<T*>(this).*action.fun)(&json);
                    }
                }
            }
            return false;
        }

        static constexpr size_t IDLE_INTERVAL = 0U;
        static constexpr const char* CONTROLLER_KEY = "controller";
        static constexpr const char* COMMAND_KEY = "command";
    };
} // namespace parser
#endif // __CONTROLLER_HPP__