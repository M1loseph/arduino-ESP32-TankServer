#include <Arduino.h>
#include <unity.h>
#include "controllers/engines_controller.hpp"

json_parser::engines_controller ec;

void test_permissions()
{
    StaticJsonDocument<256> json;
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::not_permited, ec.try_handle(json.as<JsonObjectConst>()));
    json["controller"] = "enginess";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::not_permited, ec.try_handle(json.as<JsonObjectConst>()));
    json["controller"] = "engines";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error, ec.try_handle(json.as<JsonObjectConst>()));
}

void test_handle_error()
{
    StaticJsonDocument<256> json;
    json["controller"] = "engines";
    json["command"] = "NOTEXIST";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error ,ec.try_handle(json.as<JsonObjectConst>()));
    json["command"] = "FORWARD";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error ,ec.try_handle(json.as<JsonObjectConst>()));
}

void test_forward_and_stop()
{
    StaticJsonDocument<256> json;
    json["controller"] = "engines";
    json["command"] =  "FORWARD";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::FORWARD, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::FORWARD, ec.get_direction_right());

    json["command"] = "STOP";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_right());

    json["command"] = "FORWARD";
    json["engines"] = "left";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::FORWARD, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_right());

    json["engines"] = "right";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::FORWARD, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::FORWARD, ec.get_direction_right());

    json["command"] = "STOP";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_right());
}

void test_backward_and_stop()
{
    StaticJsonDocument<256> json;
    json["controller"] = "engines";
    json["command"] =  "BACKWARD";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::BACKWARD, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::BACKWARD, ec.get_direction_right());

    json["command"] = "STOP";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_right());

    json["command"] = "BACKWARD";
    json["engines"] = "left";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::BACKWARD, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_right());

    json["command"] = "BACKWARD";
    json["engines"] = "right";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::BACKWARD, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::BACKWARD, ec.get_direction_right());

    json["command"] = "STOP";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::direction::STOP, ec.get_direction_right());
}

void test_faster_and_keep_speed()
{
    StaticJsonDocument<256> json;
    json["controller"] = "engines";
    json["command"] =  "FASTER";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::FASTER, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::FASTER, ec.get_speed_controll_right());

    json["command"] = "KEEP_SPEED";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_right());

    json["command"] = "FASTER";
    json["engines"] = "left";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::FASTER, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_right());

    json["command"] = "FASTER";
    json["engines"] = "right";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::FASTER, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::FASTER, ec.get_speed_controll_right());

    json["command"] = "KEEP_SPEED";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_right());
}

void test_slower_and_keep_speed()
{
    StaticJsonDocument<256> json;
    json["controller"] = "engines";
    json["command"] =  "SLOWER";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::SLOWER, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::SLOWER, ec.get_speed_controll_right());

    json["command"] = "KEEP_SPEED";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_right());

    json["command"] = "SLOWER";
    json["engines"] = "left";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::SLOWER, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_right());

    json["command"] = "SLOWER";
    json["engines"] = "right";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::SLOWER, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::SLOWER, ec.get_speed_controll_right());

    json["command"] = "KEEP_SPEED";
    json["engines"] = "both";
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_left());
    TEST_ASSERT_EQUAL(json_parser::engines_controller::speed_controll::KEEP_SPEED, ec.get_speed_controll_right());
}

void test_set_speed()
{
    StaticJsonDocument<256> json;
    json["controller"] = "engines";
    json["command"] =  "SPEED";
    json["engines"] = "both";
    json["speed"] = 1023U;
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL_UINT32(1023U, ec.get_speed_left());
    TEST_ASSERT_EQUAL_UINT32(1023U, ec.get_speed_right());
    
    json["engines"] = "left";
    json["speed"] = 0U;
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL_UINT32(0U, ec.get_speed_left());
    TEST_ASSERT_EQUAL_UINT32(1023U, ec.get_speed_right());

    json["engines"] = "right";
    json["speed"] = 1000U;
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::ok ,ec.try_handle(json.as<JsonObjectConst>()));
    TEST_ASSERT_EQUAL_UINT32(0U, ec.get_speed_left());
    TEST_ASSERT_EQUAL_UINT32(1000U, ec.get_speed_right());

    json.remove("speed");
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error ,ec.try_handle(json.as<JsonObjectConst>()));
    json["speed"] = 2000;
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error ,ec.try_handle(json.as<JsonObjectConst>()));
    json.remove("engines");
    TEST_ASSERT_EQUAL(json_parser::controller::handle_resoult::error ,ec.try_handle(json.as<JsonObjectConst>()));
}

void setup()
{
    delay(2000);
    ec.initialize();
    UNITY_BEGIN();
    RUN_TEST(test_permissions);
    RUN_TEST(test_handle_error);
    RUN_TEST(test_forward_and_stop);
    RUN_TEST(test_backward_and_stop);
    RUN_TEST(test_faster_and_keep_speed);
    RUN_TEST(test_slower_and_keep_speed);
    RUN_TEST(test_set_speed);
    UNITY_END();
}

void loop()
{
}