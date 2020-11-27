#ifndef __GLOBAL_QUEUE_HPP__
#define __GLOBAL_QUEUE_HPP__

#include <ArduinoJson.h>
#include <queue>

class ipc_queue {

    explicit ipc_queue()
    {
        _mutex = xSemaphoreCreateMutex();
    }

    void push(DynamicJsonDocument &&json)
    {
        while ((xSemaphoreTake(_mutex, (TickType_t)10)) != pdTRUE);
        _queue.push(std::move(json));
        xSemaphoreGive(_mutex);
    }

    bool empty()
    {
        while ((xSemaphoreTake(_mutex, (TickType_t)10)) != pdTRUE);
        bool is_empty = _queue.empty();
        xSemaphoreGive(_mutex);
        return is_empty;
    }

    DynamicJsonDocument dequeue()
    {
        while ((xSemaphoreTake(_mutex, (TickType_t)10)) != pdTRUE);
        DynamicJsonDocument  json = std::move(_queue.back());
        _queue.pop();
        xSemaphoreGive(_mutex);
        return json;
    }

private:
    xSemaphoreHandle _mutex;
    std::queue<DynamicJsonDocument> _queue;
};

#endif // __GLOBAL_QUEUE_HPP__