#include <Arduino.h>
#include <unity.h>
#include "buffer/buffer.h"

void TestAddingAndRemoving()
{
    CommandBuffer b;
    const char *alphabet = "alphabet";
    for (size_t i = 0; i < strlen(alphabet); i++)
        b.PushBack(alphabet[i]);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(alphabet, b.C_Ptr(), 7);

    b.Clear();

    char fullNulls[CommandBuffer::BUFFER_LENGTH] = {'\0'};
    TEST_ASSERT_EQUAL_CHAR_ARRAY(fullNulls, b.C_Ptr(), 100);
}

void TestNumberParsing()
{
    CommandBuffer b;
    const char *numbers = "30 20a o0   30";

    for (size_t i = 0; i < strlen(numbers); i++)
        b.PushBack(numbers[i]);

    TEST_ASSERT_EQUAL_INT(30, b.FindNumber(0).number);
    TEST_ASSERT_FALSE(b.FindNumber(1).success);
    TEST_ASSERT_FALSE(b.FindNumber(2).success);
    TEST_ASSERT_EQUAL_INT(30, b.FindNumber(3).number);

    b.Clear();

    TEST_ASSERT_FALSE(b.FindNumber(0).success);
    TEST_ASSERT_NULL(b.Command());
}

void TestCommandParsing()
{
    CommandBuffer b;
    const char *command = "MP3_COMMAND 20 30 40";

    for (size_t i = 0; i < strlen(command); i++)
        b.PushBack(command[i]);

    TEST_ASSERT_EQUAL_STRING("MP3_COMMAND", b.Command());
    TEST_ASSERT_FALSE(b.FindNumber(0).success);
    TEST_ASSERT_EQUAL_INT(20, b.FindNumber(1).number);
    TEST_ASSERT_EQUAL_INT(30, b.FindNumber(2).number);
    TEST_ASSERT_EQUAL_INT(40, b.FindNumber(3).number);
    TEST_ASSERT_FALSE(b.FindNumber(4).success);

    b.Clear();

    TEST_ASSERT_FALSE(b.FindNumber(0).success);
    TEST_ASSERT_NULL(b.Command());
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(TestAddingAndRemoving);
    RUN_TEST(TestNumberParsing);
    RUN_TEST(TestCommandParsing);
    UNITY_END();
}

void loop()
{
}