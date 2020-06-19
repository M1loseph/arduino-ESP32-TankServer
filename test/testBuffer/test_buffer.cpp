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

    char fullNulls[CommandBuffer::ST_BUFFER_LENGTH] = {'\0'};
    TEST_ASSERT_EQUAL_CHAR_ARRAY(fullNulls, b.C_Ptr(), 100);
}

void TestPositiveNumberParsing()
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
    const char *text = "Reading Jojo is kind of like those pretend fights you had when you were younger.";

    for (size_t i = 0; i < strlen(text); i++)
        b.PushBack(text[i]);

    TEST_ASSERT_EQUAL_STRING("Reading", b.Word(0));
    TEST_ASSERT_EQUAL_STRING("Jojo", b.Word(1));
    TEST_ASSERT_EQUAL_STRING("is", b.Word(2));
    TEST_ASSERT_EQUAL_STRING("kind", b.Word(3));
    TEST_ASSERT_EQUAL_STRING("of", b.Word(4));
    TEST_ASSERT_EQUAL_STRING("like", b.Word(5));
    TEST_ASSERT_EQUAL_STRING("those", b.Word(6));
    TEST_ASSERT_EQUAL_STRING("pretend", b.Word(7));
    TEST_ASSERT_EQUAL_STRING("fights", b.Word(8));
    TEST_ASSERT_EQUAL_STRING("you", b.Word(9));
    TEST_ASSERT_EQUAL_STRING("had", b.Word(10));
    TEST_ASSERT_EQUAL_STRING("when", b.Word(11));
    TEST_ASSERT_EQUAL_STRING("you", b.Word(12));
    TEST_ASSERT_EQUAL_STRING("were", b.Word(13));
    TEST_ASSERT_EQUAL_STRING("younger.", b.Word(14));
    for (size_t i = 15; i < 100; i++)
        TEST_ASSERT_NULL(b.Word(i));
}

void TestBothPositiveAndNegativeLoop()
{
    char buffer[16];
    CommandBuffer b;

    for (int i = -24; i < 20; i++)
    {
        char *number = itoa(i, buffer, 10);

        for (size_t j = 0; j < strlen(number); j++)
            b.PushBack(number[j]);

        Number m = b.FindNumber(0);
        TEST_ASSERT_TRUE(m.success);
        TEST_ASSERT_EQUAL_INT(i, m.value);
        b.Clear();
    }
    Number m = b.FindNumber(0);

    TEST_ASSERT_FALSE(m.success);
    TEST_ASSERT_EQUAL_INT(CommandBuffer::NOT_FOUND, m.value);
}

void TestBothPositiveAndNegative()
{
    int values[] = {21, -30, 1215, -3124};
    const char *text = "  21  -30 1215 -3124 --44";
    CommandBuffer b;
    for (size_t i = 0; i < strlen(text); i++)
        b.PushBack(text[i]);

    constexpr size_t length = 5;
    Number numbers[length];
    for (size_t i = 0; i < length; i++)
        numbers[i] = b.FindNumber(i);

    for (size_t i = 0; i < length - 1; i++)
        TEST_ASSERT_EQUAL_INT(values[i], numbers[i].value);

    TEST_ASSERT_FALSE(numbers[length - 1].success);
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(TestAddingAndRemoving);
    RUN_TEST(TestPositiveNumberParsing);
    RUN_TEST(TestCommandParsing);
    RUN_TEST(TestFindingWords);
    RUN_TEST(TestBothPositiveAndNegative);
    RUN_TEST(TestBothPositiveAndNegativeLoop);
    UNITY_END();
}

void loop()
{
}