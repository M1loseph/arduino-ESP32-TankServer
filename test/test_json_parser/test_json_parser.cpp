#include <Arduino.h>
#include <unity.h>
#include "json_parser/parser.hpp"
#include "controllers/arm_controller.hpp"
#include "controllers/engines_controller.hpp"
#include "controllers/leds_controller.hpp"
#include "controllers/mp3_controller.hpp"
#include "controllers/sd_controller.hpp"

void test_adding()
{
    json_parser::parser parser;
    TEST_ASSERT_TRUE(parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::arm_controller())));
    TEST_ASSERT_TRUE(parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::engines_controller())));
    TEST_ASSERT_TRUE(parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::leds_controller())));
    TEST_ASSERT_TRUE(parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::mp3_controller())));
    TEST_ASSERT_TRUE(parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::sd_controller())));
}

void test_handle_resoult()
{
    json_parser::parser parser;
    parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::arm_controller()));
    TEST_ASSERT_TRUE(parser.initialize_all());
    StaticJsonDocument<256> json;
    json["controller"] = "arm";
    json["command"] = "STOP";
    json["servo"] = "BASE";
    auto res = parser.handle(json.as<JsonObjectConst>());
    TEST_ASSERT_EQUAL_UINT8(1, res.first);
    TEST_ASSERT_EQUAL_UINT8(1, res.second);
    json["servo"] = "DONT EXISTS";
    res = parser.handle(json.as<JsonObjectConst>());
    TEST_ASSERT_EQUAL_UINT8(1, res.first);
    TEST_ASSERT_EQUAL_UINT8(0, res.second);
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_adding);
    RUN_TEST(test_handle_resoult);
    UNITY_END();
}

void loop()
{
}