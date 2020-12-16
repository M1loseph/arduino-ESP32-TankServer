#include "controller.hpp"

namespace json_parser
{
    controller::handle_resoult controller::try_handle(const JsonObject &json)
    {
        if (can_handle(json))
        {
            return handle(json) ? handle_resoult::ok : handle_resoult::error;
        }
        return handle_resoult::not_permited;
    }
} // namespace json_parser
