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
            _semaphore = xSemaphoreCreateMutex();
            return _queue && _semaphore;
        }

        bool push(DynamicJsonDocument **json)
        {
            if (xSemaphoreTake(_semaphore, (TickType_t)10) == pdTRUE)
            {
                bool succ = xQueueSend(_queue, json, (TickType_t)0) == pdPASS;
                xSemaphoreGive(_semaphore);
                return succ;
            }
            return false;
        }

        bool read(DynamicJsonDocument **json)
        {
            return xQueueReceive(_queue, json, (TickType_t)0) == pdPASS;
        }

    private:
        xQueueHandle _queue;
        SemaphoreHandle_t _semaphore;
    };

    extern global_queue queue;
} // namespace global_queue

#endif // __GLOBAL_QUEUE_HPP__