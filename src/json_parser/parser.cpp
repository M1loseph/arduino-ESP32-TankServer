#include "parser.hpp"
#include "debug.hpp"

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
        LOG_PARSER_NL("[parser] trying to handle...")
        uint8_t count = 0;
        for (const auto &controller : _controllers)
        {
            count += controller->try_handle(json) == controller::handle_resoult::ok;
        }
        LOG_PARSER_F("[parser] JSON handled %d times\n", count)
        return count;
    }

    void parser::hadnle_updates()
    {
        for(const auto& controller : _controllers)
            controller->update();
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

    bool parser::initialize_all()
    {
        if(!_controllers.size()) return false;

        bool res = true;
        for(auto& controller : _controllers)
        {
            LOG_PARSER_NL("[parser] initializing engine pins...");
            bool init_res = controller->initialize();
            LOG_PARSER_F("[parser] initing %s: %s\n", controller->get_name(), init_res ? "successful" : "error");

            res &= init_res;
        }
        return res;
    }
} // namespace parser
