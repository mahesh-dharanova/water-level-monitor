#include <Arduino.h>
#include "sensor.h"
#include "config.h"

void sensorInit() {
    pinMode(SENSOR_OUT_PIN, INPUT);
}

bool sensorReadRaw() {
    return digitalRead(SENSOR_OUT_PIN) == HIGH;
}
