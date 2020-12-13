#include <Arduino.h>
#include <SD.h>
#include "abstract/controller.hpp"

namespace json_parser
{
    class sd_controller final : public controller
    {
    public:
        explicit sd_controller();
        ~sd_controller();
        bool initialize() override;
        void update() override;
        DynamicJsonDocument retrive_data() override;

    private:
        bool can_handle(const JsonObject &json) const override;
        bool handle(const JsonObject &json) override;
        void handle_current_json();
        void delete_json();

        static constexpr uint8_t CHIP_SELECT = 5U;
        static constexpr const char *EXECUTE = "execute";
        static constexpr const char *FILE_KEY = "file";

        static constexpr const char *LOG_FILE = "/logs.txt";

        static constexpr const char *TIME_KEY = "time";

        File _file;
        bool _execute;
        String _file_to_execute;

        unsigned long _last_log;
        unsigned long _last_executed;
        DynamicJsonDocument *_json;
    };
} // namespace json_parser