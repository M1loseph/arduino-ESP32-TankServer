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

    char fullNulls[CommandBuffer::BUFFER_MAX_LENGTH] = {'\0'};
    TEST_ASSERT_EQUAL_CHAR_ARRAY(fullNulls, b.C_Ptr(), CommandBuffer::BUFFER_MAX_LENGTH);
}

void TestPositiveNumberParsing()
{
    CommandBuffer b;
    const char *numbers = "30 20a o0   30";

    for (size_t i = 0; i < strlen(numbers); i++)
        b.PushBack(numbers[i]);

    TEST_ASSERT_EQUAL_INT(30, b.IntAt(0).value);
    TEST_ASSERT_FALSE(b.IntAt(1).success);
    TEST_ASSERT_FALSE(b.IntAt(2).success);
    TEST_ASSERT_EQUAL_INT(30, b.IntAt(3).value);

    b.Clear();

    TEST_ASSERT_FALSE(b.IntAt(0).success);
    TEST_ASSERT_NULL(b.Command());
}

void TestCommandParsing()
{
    CommandBuffer b;
    const char *command = "MP3_COMMAND 20 30 40";

    for (size_t i = 0; i < strlen(command); i++)
        b.PushBack(command[i]);

    TEST_ASSERT_EQUAL_STRING("MP3_COMMAND", b.Command());
    TEST_ASSERT_FALSE(b.IntAt(0).success);
    TEST_ASSERT_EQUAL_INT(20, b.IntAt(1).value);
    TEST_ASSERT_EQUAL_INT(30, b.IntAt(2).value);
    TEST_ASSERT_EQUAL_INT(40, b.IntAt(3).value);
    TEST_ASSERT_FALSE(b.IntAt(4).success);

    b.Clear();

    TEST_ASSERT_FALSE(b.IntAt(0).success);
    TEST_ASSERT_NULL(b.Command());
}

void TestFindingWords()
{
    CommandBuffer b;
    const char *text = "Reading Jojo is kind of like those pretend fights you had when you were younger.";

    for (size_t i = 0; i < strlen(text); i++)
        b.PushBack(text[i]);

    TEST_ASSERT_EQUAL_STRING("Reading", b.WordAt(0));
    TEST_ASSERT_EQUAL_STRING("Jojo", b.WordAt(1));
    TEST_ASSERT_EQUAL_STRING("is", b.WordAt(2));
    TEST_ASSERT_EQUAL_STRING("kind", b.WordAt(3));
    TEST_ASSERT_EQUAL_STRING("of", b.WordAt(4));
    TEST_ASSERT_EQUAL_STRING("like", b.WordAt(5));
    TEST_ASSERT_EQUAL_STRING("those", b.WordAt(6));
    TEST_ASSERT_EQUAL_STRING("pretend", b.WordAt(7));
    TEST_ASSERT_EQUAL_STRING("fights", b.WordAt(8));
    TEST_ASSERT_EQUAL_STRING("you", b.WordAt(9));
    TEST_ASSERT_EQUAL_STRING("had", b.WordAt(10));
    TEST_ASSERT_EQUAL_STRING("when", b.WordAt(11));
    TEST_ASSERT_EQUAL_STRING("you", b.WordAt(12));
    TEST_ASSERT_EQUAL_STRING("were", b.WordAt(13));
    TEST_ASSERT_EQUAL_STRING("younger.", b.WordAt(14));
    for (size_t i = 15; i < 100; i++)
        TEST_ASSERT_NULL(b.WordAt(i));
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

        Integer m = b.IntAt(0);
        TEST_ASSERT_TRUE(m.success);
        TEST_ASSERT_EQUAL_INT(i, m.value);
        b.Clear();
    }
    Integer m = b.IntAt(0);

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
    Integer numbers[length];
    for (size_t i = 0; i < length; i++)
        numbers[i] = b.IntAt(i);

    for (size_t i = 0; i < length - 1; i++)
        TEST_ASSERT_EQUAL_INT(values[i], numbers[i].value);

    TEST_ASSERT_FALSE(numbers[length - 1].success);
}

void TestFloats()
{
    CommandBuffer b;
    constexpr size_t floatLength = 6;
    float floats[] = {1.0, 2.0, 3.0, 3.5, 10.5, 20.0};
    const char *text = " 1.0  2.0  3.0 3.5 10.5 20.0  2.  3  2. .0 10..";

    for (size_t i = 0; i < strlen(text); i++)
        b.PushBack(text[i]);

    for (size_t i = 0; i < floatLength; i++)
    {
        Float f = b.FloatAt(i);
        TEST_ASSERT_EQUAL_FLOAT(floats[i], f.value);
    }

    for (size_t i = floatLength; i < floatLength + 10; i++)
    {
        Float f = b.FloatAt(i);
        TEST_ASSERT_FALSE(f.success);
    }
}

void TestNegativeFloats()
{
    CommandBuffer b;
    constexpr size_t floatLength = 6;
    float floats[] = {-1.0, -2.0, -3.0, -3.5, -10.5, -20.0};
    const char *text = " -1.0  -2.0  -3.0 -3.5 -10.5 -20.0  -2.  -3  -2. -.0 --10.. -9";

    for (size_t i = 0; i < strlen(text); i++)
        b.PushBack(text[i]);

    for (size_t i = 0; i < floatLength; i++)
    {
        Float f = b.FloatAt(i);
        TEST_ASSERT_EQUAL_FLOAT(floats[i], f.value);
    }

    for (size_t i = floatLength; i < floatLength + 10; i++)
    {
        Float f = b.FloatAt(i);
        TEST_ASSERT_FALSE(f.success);
    }
}

void TestLongNumbers()
{
    CommandBuffer b;
    const char *text = "2000 3000 4000 5000 10000";
    int trueResoults[] = {2000, 3000, 4000, 5000, 10000};

    for(size_t i = 0; i < strlen(text); i++)
        b.PushBack(text[i]);

    for(size_t i = 0; i < 5; i++)
        TEST_ASSERT_EQUAL(trueResoults[i], b.IntAt(i).value);
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
    RUN_TEST(TestFloats);
    RUN_TEST(TestNegativeFloats);
    RUN_TEST(TestLongNumbers);
    UNITY_END();
}

void loop()
{
}