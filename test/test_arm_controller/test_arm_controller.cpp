#include <Arduino.h>
#include <unity.h>
#include "controllers/arm_controller.hpp"

json_parser::arm_controller ac;
const char *servo_names[] = {
    "BASE",
    "SHOULDER",
    "ELBOW",
    "WRIST",
    "ROTATION",
    "CLAW",
};


void test_permission()
{
    StaticJsonDocument<256> json;
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::not_permited, ac.try_handle(json.as<JsonObjectConst>()));
    json["controller"] = "armmm";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::not_permited, ac.try_handle(json.as<JsonObjectConst>()));
    json["controller"] = "arm";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error, ac.try_handle(json.as<JsonObjectConst>()));
}

void test_servo_plus_and_minus()
{
    for(const char* servo_name : servo_names)
    {
        auto servo = ac.get_servo_by_name(servo_name);
        TEST_ASSERT_NOT_NULL(servo);
        StaticJsonDocument<256> json;
        json["controller"] = "arm";
        json["command"] = "PLUS";
        json["servo"] = servo_name;
        TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok, ac.try_handle(json.as<JsonObjectConst>()));
        TEST_ASSERT_EQUAL(servo->MAX_ANGLE, servo->destination_angle);

        json["command"] = "STOP";
        TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok, ac.try_handle(json.as<JsonObjectConst>()));
        TEST_ASSERT_EQUAL(servo->current_angle, servo->destination_angle);

        json["command"] = "MINUS";
        TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok, ac.try_handle(json.as<JsonObjectConst>()));
        TEST_ASSERT_EQUAL(servo->MIN_ANGLE, servo->destination_angle);

        json["command"] = "STOP";
        TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok, ac.try_handle(json.as<JsonObjectConst>()));
        TEST_ASSERT_EQUAL(servo->current_angle, servo->destination_angle);
    }
}

void test_set_angle()
{
    for(const char* servo_name : servo_names)
    {
        auto servo = ac.get_servo_by_name(servo_name);
        TEST_ASSERT_NOT_NULL(servo);
        StaticJsonDocument<256> json;
        json["controller"] = "arm";
        json["command"] = "ANGLE";
        json["servo"] = servo_name;

        if(servo->MIN_ANGLE != 0)
        {
            json["angle"] = servo->MIN_ANGLE - 1U;
            TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error, ac.try_handle(json.as<JsonObjectConst>()));
            TEST_ASSERT_EQUAL(servo->current_angle, servo->destination_angle);
        }
        json["angle"] = servo->MAX_ANGLE + 1;
        TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error, ac.try_handle(json.as<JsonObjectConst>()));
        TEST_ASSERT_EQUAL(servo->current_angle, servo->destination_angle);

        json["angle"] = servo->MAX_ANGLE;
        TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok, ac.try_handle(json.as<JsonObjectConst>()));
        TEST_ASSERT_EQUAL(servo->MAX_ANGLE, servo->destination_angle);

        json["angle"] = servo->MIN_ANGLE;
        TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok, ac.try_handle(json.as<JsonObjectConst>()));
        TEST_ASSERT_EQUAL(servo->MIN_ANGLE, servo->destination_angle);
    }
}

void setup()
{
    delay(2000);
    ac.initialize();
    UNITY_BEGIN();
    RUN_TEST(test_permission);
    RUN_TEST(test_servo_plus_and_minus);
    RUN_TEST(test_set_angle);
    UNITY_END();
}

void loop()
{
}