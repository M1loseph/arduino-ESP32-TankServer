#include <Arduino.h>
#include <unity.h>
#include "parser/parser.h"
#include "buffer/buffer.h"
#include "commands.h"

size_t zmienna = 0;

static void Forward(const CommandBuffer &b)
{
    for (size_t i = 0; i < 10; i++)
        TEST_ASSERT_FALSE(b.IntAt(i).success);
    zmienna += 1;
}

void TestCommandFound()
{
    Parser parser;
    constexpr size_t length = 7;

    const char *events[length] = {
        Command::Mcu::TANK_FORWARD_L,
        Command::Mcu::TANK_BACKWARD_R,
        Command::Mcu::TANK_FASTER,
        Command::Mcu::TANK_SLOWER,
        Command::Mcu::TANK_STEADY,
        Command::Mcu::TANK_STOP_L,
        Command::Mcu::TANK_STOP_R};

    for (size_t i = 0; i < length; i++)
        parser.AddEvent(events[i], Forward);

    for (int i = 0; i < 1000; i++)
    {
        const char *command = events[random(length)];
        for (size_t i = 0; i < strlen(command) + 1; i++)
            parser.GetBuff().PushBack(command[i]);

        parser.ExecuteMessege();
        TEST_ASSERT_EQUAL(i + 1, zmienna);
    }
    parser.ExecuteMessege();
}

void TestSettingInterval()
{
    Parser p;
    p.AddEvent(Command::Mega::MP3_COMMAND, Forward);
    p.AddEvent(Command::Mega::ELBOW_2, Forward);

    TEST_ASSERT_TRUE(p.SetInterval(Command::Mega::MP3_COMMAND, 30));
    TEST_ASSERT_TRUE(p.SetInterval(Command::Mega::ELBOW_2, 40));

    TEST_ASSERT_EQUAL(30, p.GetInterval(Command::Mega::MP3_COMMAND));
    TEST_ASSERT_EQUAL(40, p.GetInterval(Command::Mega::ELBOW_2));

    TEST_ASSERT_EQUAL(Parser::INTERVAL_NOT_FOUND, p.GetInterval(Command::Common::MOVE));
    TEST_ASSERT_FALSE(p.SetInterval(Command::Mega::SHOULDER, 0));
}

void setup()
{
    delay(2000);
    randomSeed(analogRead(0));
    UNITY_BEGIN();
    RUN_TEST(TestSettingInterval);
    RUN_TEST(TestCommandFound);
    UNITY_END();
}

void loop()
{
}