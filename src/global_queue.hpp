#ifndef __GLOBAL_QUEUE_HPP__
#define __GLOBAL_QUEUE_HPP__

#include <ArduinoJson.h>
#include <queue>

namespace global_queue
{
    class global_queue
    {
    public:
        bool initialize()
        {
            _queue = xQueueCreate(10, sizeof(DynamicJsonDocument *));
            return _queue != nullptr;
        }

        bool push(DynamicJsonDocument **json)
        {
            return xQueueSend(_queue, json, (TickType_t)0) == pdPASS;
        }

        bool read(DynamicJsonDocument **json)
        {
            return xQueueReceive(_queue, json, (TickType_t)0) == pdPASS;
        }

    private:
        xQueueHandle _queue;
    };

    extern global_queue queue;
} // namespace global_queue

#endif // __GLOBAL_QUEUE_HPP__