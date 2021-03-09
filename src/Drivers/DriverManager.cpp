#include "Drivers/DriverManager.h"

DriverManager::DriverManager() {

}

DriverManager::~DriverManager() {

}

void DriverManager::addDriver(Driver *driver) {
    drivers[drivers_num] = driver;
    drivers_num++;
}

void DriverManager::activateDrivers() {
    for (uint32_t i = 0; i < drivers_num; i++) {
        drivers[i]->activate();
    }
}
