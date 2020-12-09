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
        DynamicJsonDocument retrive_data() override;

    private:
        bool can_handle(const JsonObject &json) const override;
        bool handle(const JsonObject& json) override;

        static constexpr uint8_t CHIP_SELECT = 5U;
        static constexpr const char* LOG_KEY = "log";

        static constexpr const char *SCRIPT_FILE = "/script.txt";
        static constexpr const char *LOG_FILE = "/logs.txt";
        static constexpr const char *TIME_KEY = "time";

        File _file;
    };
} // namespace sd_card