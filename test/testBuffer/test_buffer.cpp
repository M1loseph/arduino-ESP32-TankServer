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

    TEST_ASSERT_EQUAL_INT(30, b.FindNumber(0).value);
    TEST_ASSERT_FALSE(b.FindNumber(1).success);
    TEST_ASSERT_FALSE(b.FindNumber(2).success);
    TEST_ASSERT_EQUAL_INT(30, b.FindNumber(3).value);

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
    TEST_ASSERT_EQUAL_INT(20, b.FindNumber(1).value);
    TEST_ASSERT_EQUAL_INT(30, b.FindNumber(2).value);
    TEST_ASSERT_EQUAL_INT(40, b.FindNumber(3).value);
    TEST_ASSERT_FALSE(b.FindNumber(4).success);

    b.Clear();

    TEST_ASSERT_FALSE(b.FindNumber(0).success);
    TEST_ASSERT_NULL(b.Command());
}

void TestFindingWords()
{
    CommandBuffer b;
    const char *text = "Reading Jojo is kind of like those pretend fights you had when you were .";

    for(size_t i = 0; i < strlen(text); i++)
        b.PushBack(text[i]);

    TEST_ASSERT_EQUAL_STRING(b.Word(0), "Reading");
    TEST_ASSERT_EQUAL_STRING(b.Word(1), "Jojo");
    TEST_ASSERT_EQUAL_STRING(b.Word(2), "is");
    TEST_ASSERT_EQUAL_STRING(b.Word(3), "kind");
    TEST_ASSERT_EQUAL_STRING(b.Word(4), "of");
    TEST_ASSERT_EQUAL_STRING(b.Word(5), "like");
    TEST_ASSERT_EQUAL_STRING(b.Word(6), "those");
    TEST_ASSERT_EQUAL_STRING(b.Word(7), "pretend");
    TEST_ASSERT_EQUAL_STRING(b.Word(8), "fights");
    TEST_ASSERT_EQUAL_STRING(b.Word(9), "you");
    TEST_ASSERT_EQUAL_STRING(b.Word(10), "had");
    TEST_ASSERT_EQUAL_STRING(b.Word(11), "when");
    TEST_ASSERT_EQUAL_STRING(b.Word(12), "you");
    TEST_ASSERT_EQUAL_STRING(b.Word(13), "were");
    TEST_ASSERT_EQUAL_STRING(b.Word(14), "younger.");
    for(size_t i = 15; i < 100; i++)
        TEST_ASSERT_NULL(b.Word(i));
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