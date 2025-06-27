#include "ScalesTask.h"

ScalesTask::ScalesTask() {
    xTaskCreate([](void *arg) {
        ScalesTask *scalesTask = static_cast<ScalesTask*>(arg);

        while (true) {
            scalesTask->weight = scalesTask->scales.getWeight();
            delay(200);
        }
    }, "ScalesTask", 2048, this, 1, &taskHandle);
}

ScalesTask::~ScalesTask() {
    if (taskHandle) {
        vTaskDelete(taskHandle);
        taskHandle = nullptr;
    }
}

float ScalesTask::getWeight() const {
    return weight.load();
}

void ScalesTask::zero() {
    scales.setOffset();
}
