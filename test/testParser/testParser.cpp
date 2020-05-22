#include <Arduino.h>
#include <unity.h>
#include "parser/parser.h"
#include "buffer/buffer.h"


int variable = 0;

static void Test(const CommandBuffer& b)
{
    for(size_t i = 0; i < 10; i++)
        TEST_ASSERT_FALSE(b.FindNumber(i).success);
    variable = 1;
}

void testCommandFound()
{
    const char* command = "TEST";
    Parser p;
    p.AddEvents(command, Test);
    for(size_t i = 0; i < strlen(command); i++)
        p.GetBuff().PushBack(command[i]);

    p.ExecuteMessege();

    TEST_ASSERT_EQUAL_INT(1, variable);
}


void setup()
{
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(testCommandFound);
    UNITY_END();
}

void loop()
{
}