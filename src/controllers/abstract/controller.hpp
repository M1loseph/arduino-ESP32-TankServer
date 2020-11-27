#ifndef __ICONTROLLER_HPP__
#define __ICONTROLLER_HPP__

#include <ArduinoJson.h>

namespace json_parser
{
    class controller
    {
    public:
        enum class handle_resoult
        {
            ok,
            not_permited,
            error
        };

        controller(const char *name) : _name(name) {}
        virtual ~controller() = default;

        virtual handle_resoult try_handle(const JsonObjectConst &json)
        {
            if (can_handle(json))
            {
                return handle(json) ? handle_resoult::ok : handle_resoult::error;
            }
            return handle_resoult::not_permited;
        }
        inline const char *get_name() { return _name; }

        virtual void update() = 0;
        virtual bool initialize() = 0;

    protected:
        virtual bool can_handle(const JsonObjectConst &json) const = 0;
        const char *const _name;

    private:
        virtual bool handle(const JsonObjectConst &json) = 0;
    };
} // namespace json_parser
#endif // __ICONTROLLER_HPP__