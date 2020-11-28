#include <Arduino.h>
#include <unity.h>
#include "controllers/leds_controller.hpp"

json_parser::leds_controller lc;

void test_permissions()
{
    StaticJsonDocument<256> json;
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::not_permited, lc.try_handle(json.as<JsonObjectConst>()));
    json["controller"] = "ledss";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::not_permited, lc.try_handle(json.as<JsonObjectConst>()));
    json["controller"] = "leds";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error, lc.try_handle(json.as<JsonObjectConst>()));
}

void test_handle_error()
{
    StaticJsonDocument<256> json;
    json["controller"] = "leds";
    json["command"] = "NOTEXIST";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error ,lc.try_handle(json.as<JsonObjectConst>()));
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_permissions);
    RUN_TEST(test_handle_error);
    UNITY_END();
}

void loop()
{

}