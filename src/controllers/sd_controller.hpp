#include <Arduino.h>
#include <SD.h>
#include "abstract/controller.hpp"

namespace json_parser
{
    class sd_controller final : public controller
    {
    public:

        explicit sd_controller();
        bool initialize() override;
        void update() override
        {}

    private:
        bool can_handle(const JsonObjectConst &json) const override;
        bool handle(const JsonObjectConst& json) override;

        static constexpr uint8_t CHIP_SELECT = 5U;
        static constexpr const char* LOG_KEY = "log";

        static constexpr const char *SCRIPT_FILE = "/script.txt";
        static constexpr const char *LOG_FILE = "/logs.txt";

        File _file;
    };
} // namespace sd_card