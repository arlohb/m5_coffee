#include "ScalesTask.h"

#include "HX711Scales.h"

ScalesTask::ScalesTask() : scales(std::make_unique<HX711Scales>()) {
    xTaskCreate([](void *arg) {
        ScalesTask *scalesTask = static_cast<ScalesTask*>(arg);

        while (true) {
            scalesTask->weight = scalesTask->scales->getWeight();
            delay(50);
        }
    }, "ScalesTask", 4 * 1024, this, 1, &taskHandle);
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
    scales->zero();
}
