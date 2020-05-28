#include <Arduino.h>
#include <unity.h>
#include "parser/parser.h"
#include "buffer/buffer.h"
#include "nodemcu/commands.h"

size_t zmienna = 0;

static void Forward(const CommandBuffer &b)
{
    for (size_t i = 0; i < 10; i++)
        TEST_ASSERT_FALSE(b.FindNumber(i).success);
    zmienna += 1;
}

void testCommandFound()
{
    Parser parser;
    const char *events[9] = {
        Command::FORWARD,
        Command::BACKWARD,
        Command::LEFT,
        Command::RIGHT,
        Command::STOP,
        Command::SPEED,
        Command::TURRET_FORWARD,
        Command::TURRET_BACKWARD,
        Command::TURRET_STOP};

    parser.AddEvents(Command::FORWARD, Forward);
    parser.AddEvents(Command::BACKWARD, Forward);
    parser.AddEvents(Command::LEFT, Forward);
    parser.AddEvents(Command::RIGHT, Forward);
    parser.AddEvents(Command::STOP, Forward);
    parser.AddEvents(Command::SPEED, Forward);
    parser.AddEvents(Command::TURRET_FORWARD, Forward);
    parser.AddEvents(Command::TURRET_BACKWARD, Forward);
    parser.AddEvents(Command::TURRET_STOP, Forward);

    for (int i = 0; i < 1000; i++)
    {
        const char *command = events[random(9)];
        for (size_t i = 0; i < strlen(command) + 1; i++)
            parser.GetBuff().PushBack(command[i]);

        parser.ExecuteMessege();
        TEST_ASSERT_EQUAL(i + 1, zmienna);
    }
    parser.ExecuteMessege();
}

void setup()
{
    delay(2000);
    randomSeed(analogRead(0));
    UNITY_BEGIN();
    RUN_TEST(testCommandFound);
    UNITY_END();
}

void loop()
{
}