#pragma once // NO_LINT
#include <iostream>
#include <thread>
#include <chrono>
#include "icontrollers.h"

void getNewDataByTimer(controllers::IUpdateDataController* update_controller);