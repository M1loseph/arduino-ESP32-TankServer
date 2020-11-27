#include "parser.hpp"

#if PARSER_DEBUG

#define LOG_PARSER(message) LOG(message);
#define LOG_PARSER_NL(message) LOG_NL(message);
#define LOG_PARSER_F(...) LOG_F(__VA_ARGS__);

#else

#define LOG_PARSER(message)
#define LOG_PARSER_NL(message)
#define LOG_PARSER_F(...)

#endif

namespace json_parser
{
    uint8_t parser::handle(const JsonObjectConst& json)
    {
        LOG_PARSER_NL("Trying to handle...")
        uint8_t count = 0;
        for (const auto &controller : _controllers)
        {
            count += controller->try_handle(json) == controller::handle_resoult::ok;
        }
        LOG_PARSER_F("JSON handled %d times", count)
        return count;
    }

    bool parser::add_controller(std::unique_ptr<controller> &&controller)
    {
        if (controller)
        {
            _controllers.push_back(std::move(controller));
            return true;
        }
        return false;
    }
} // namespace parser
