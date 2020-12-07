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
    std::pair<uint8_t, uint8_t> parser::handle(const JsonObjectConst &json) const
    {
        uint8_t permited = 0;
        uint8_t handled = 0;
        LOG_PARSER_NL("[parser] trying to handle...")
        for (const auto &controller : _controllers)
        {
            auto res = controller->try_handle(json);
            if (res == controller::handle_resoult::error)
            {
                permited++;
            }

            if (res == controller::handle_resoult::ok)
            {
                permited++;
                handled++;
            }
        }
        LOG_PARSER_F("[parser] permited: %d handled: %d\n", permited, handled)
        return {permited, handled};
    }

    void parser::handle_updates() const
    {
        for (const auto &controller : _controllers)
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

    bool parser::initialize_all() const
    {
        if (!_controllers.size())
            return false;

        bool res = true;
        for (auto &controller : _controllers)
        {
            LOG_PARSER_NL("[parser] initializing engine pins...");
            bool init_res = controller->initialize();
            LOG_PARSER_F("[parser] initing %s: %s\n", controller->get_name(), init_res ? "successful" : "error");

            res &= init_res;
        }
        return res;
    }

    DynamicJsonDocument parser::retrive_data() const
    {
        DynamicJsonDocument json(3000);
        JsonArray array = json.createNestedArray();
        for (auto &controller : _controllers)
        {
            array.add(controller->retrive_data());
        }
        return json;
    }
} // namespace json_parser
