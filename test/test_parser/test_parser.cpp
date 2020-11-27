#include <Arduino.h>
#include <unity.h>
#include "string_parser/string_parser.hpp"
#include "command_buffer/command_buffer.hpp"
#include "unit_test_commands.hpp"

// ================
// global variables for testing -> lambdas dont work
// ================

int test_int = 0;
bool test_bool = false;

void set_test_bool_false(const string_parser::command_buffer *)
{
    test_bool = false;
}

void set_test_bool_true(const string_parser::command_buffer *)
{
    test_bool = true;
}

void incr(const string_parser::command_buffer *)
{
    test_int++;
}

void decr(const string_parser::command_buffer *)
{
    test_int--;
}

// ================
// TESTS
// ================

void test_command_duplication()
{
    string_parser::string_parser p;
    // duplicated strings will not be tolerated
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::ONE, decr));
    TEST_ASSERT_FALSE(p.add_event(unit_test_commands::ONE, incr));

    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::TEN, decr));
    TEST_ASSERT_FALSE(p.add_event(unit_test_commands::TEN, incr));
}

void test_find_existing_commands()
{
    string_parser::string_parser p;
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::ONE, decr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::TWO, decr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::THREE, decr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::FOUR, decr));

    // these commands were added, so they should not be NULL
    TEST_ASSERT_NOT_NULL(p.get_event(unit_test_commands::ONE));
    TEST_ASSERT_NOT_NULL(p.get_event(unit_test_commands::TWO));
    TEST_ASSERT_NOT_NULL(p.get_event(unit_test_commands::THREE));
    TEST_ASSERT_NOT_NULL(p.get_event(unit_test_commands::FOUR));
}

void test_find_not_existing_commands()
{
    string_parser::string_parser p;
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::ONE, decr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::TWO, decr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::THREE, decr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::FOUR, decr));

    // these commands were added, so they should not be NULL
    TEST_ASSERT_NULL(p.get_event(unit_test_commands::FIVE));
    TEST_ASSERT_NULL(p.get_event(unit_test_commands::SIX));
    TEST_ASSERT_NULL(p.get_event(unit_test_commands::SEVEN));
    TEST_ASSERT_NULL(p.get_event(unit_test_commands::EIGHT));
    TEST_ASSERT_NULL(p.get_event(unit_test_commands::NINE));
    TEST_ASSERT_NULL(p.get_event(unit_test_commands::TEN));
}

void test_execute_command_found()
{
    string_parser::string_parser parser;
    string_parser::command_buffer b;

    TEST_ASSERT_TRUE(parser.add_event(unit_test_commands::ONE, set_test_bool_true));

    TEST_ASSERT_TRUE(b.push_back(unit_test_commands::ONE));

    // start with false
    test_bool = false;
    TEST_ASSERT_FALSE(test_bool);
    TEST_ASSERT_TRUE(parser.exec_buffer(&b));
    // after executing funtion the variable should be true
    TEST_ASSERT_TRUE(test_bool);
}

void test_execute_command_not_found()
{
    // the same test as above, but this time we try to execute different function
    // it should have no effect
    string_parser::string_parser parser;
    string_parser::command_buffer b;

    parser.add_event(unit_test_commands::ONE, set_test_bool_true);

    TEST_ASSERT_TRUE(b.push_back(unit_test_commands::TWO));

    test_bool = false;
    TEST_ASSERT_FALSE(test_bool);
    TEST_ASSERT_FALSE(parser.exec_buffer(&b));
    TEST_ASSERT_FALSE(test_bool);
}

void test_set_correct_interval()
{
    string_parser::string_parser p;

    constexpr int one_interval = 30;
    constexpr int two_interval = 40;
    constexpr int three_interval = 100;

    // setup intervals using these two methods
    // while adding
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::ONE, incr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::TWO, incr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::THREE, incr, three_interval));
    // and by calling this function
    TEST_ASSERT_TRUE(p.set_interval(unit_test_commands::ONE, one_interval));
    TEST_ASSERT_TRUE(p.set_interval(unit_test_commands::TWO, two_interval));

    TEST_ASSERT_NOT_NULL(p.get_event(unit_test_commands::ONE));
    TEST_ASSERT_NOT_NULL(p.get_event(unit_test_commands::TWO));
    TEST_ASSERT_EQUAL_INT(one_interval, p.get_event(unit_test_commands::ONE)->interval);
    TEST_ASSERT_EQUAL_INT(two_interval, p.get_event(unit_test_commands::TWO)->interval);
}

void test_set_incorrect_interval()

{
    string_parser::string_parser p;
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::ONE, incr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::TWO, incr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::THREE, incr));

    TEST_ASSERT_FALSE(p.set_interval(unit_test_commands::FIVE, 1));
    TEST_ASSERT_FALSE(p.set_interval(unit_test_commands::SIX, 1));
    TEST_ASSERT_FALSE(p.set_interval(unit_test_commands::SEVEN, 1));
    TEST_ASSERT_FALSE(p.set_interval(unit_test_commands::EIGHT, 1));
    TEST_ASSERT_FALSE(p.set_interval(unit_test_commands::NINE, 1));
}

void test_real_life_example()
{
    string_parser::string_parser p;
    string_parser::command_buffer b;

    test_int = 0;

    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::ONE, incr));
    TEST_ASSERT_TRUE(p.add_event(unit_test_commands::TWO, decr));

    TEST_ASSERT_TRUE(b.push_back(unit_test_commands::ONE));
    TEST_ASSERT_TRUE(p.exec_buffer(&b));
    TEST_ASSERT_FALSE(p.exec_buffer(&b, 1));
    b.clear();
    TEST_ASSERT_FALSE(p.exec_buffer(&b));
    // after the first exec the buffer should be empty
    TEST_ASSERT_EQUAL_INT(test_int, 1);

    b.push_back(unit_test_commands::THREE);
    TEST_ASSERT_FALSE(p.exec_buffer(&b));
    TEST_ASSERT_FALSE(p.exec_buffer(&b, 1));
    b.clear();
    // nothing should have happened
    TEST_ASSERT_EQUAL_INT(test_int, 1);

    b.push_back(unit_test_commands::TWO);
    TEST_ASSERT_TRUE(p.exec_buffer(&b));
    TEST_ASSERT_TRUE(p.exec_buffer(&b));
    // in should be -1 now
    TEST_ASSERT_EQUAL_INT(test_int, -1);
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_command_duplication);
    RUN_TEST(test_find_existing_commands);
    RUN_TEST(test_find_not_existing_commands);
    RUN_TEST(test_execute_command_found);
    RUN_TEST(test_execute_command_not_found);
    RUN_TEST(test_set_correct_interval);
    RUN_TEST(test_set_incorrect_interval);
    RUN_TEST(test_real_life_example);
    UNITY_END();
}

void loop()
{
}