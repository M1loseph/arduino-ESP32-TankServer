#include <Arduino.h>
#include <unity.h>
#include "parser/parser.h"
#include "buffer/buffer.h"
#include "commands.h"

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
        Command::Mcu::TANK_FORWARD,
        Command::Mcu::TURRET_BACKWARD,
        Command::Mcu::TANK_LEFT,
        Command::Mcu::TANK_RIGHT,
        Command::Mcu::TANK_STOP,
        Command::Mcu::TANK_SPEED,
        Command::Mcu::TURRET_FORWARD,
        Command::Mcu::TURRET_BACKWARD,
        Command::Mcu::TURRET_STOP};

    for(size_t i = 0; i < 9; i++)
        parser.AddEvents(events[i], Forward);

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