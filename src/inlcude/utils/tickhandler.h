#pragma once

#ifndef TICKHANDLER_H
#define TICKHANDLER_H

#include "metricshandler.h"
#include "minermanager.h"
#include "stationmanager.h"
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

#define MAX_TICK 864

class TickHandler
{
public:
    TickHandler(MinerManager &minerManager, StationManager &stationManager, unsigned int interval_ms);
    ~TickHandler();
    void start();
    void stop();

private:
    MinerManager &minerManager;
    StationManager &stationManager;
    unsigned int interval_ms_;
    std::vector<std::thread> timerThreads_;
    std::atomic<bool> keepRunning_;

    void tick(int id);
    void MinierMetrics(Miner &miner, int id);
    void StationMetrics(Station &station, int id, double load);
};

#endif // TICKHANDLER_H
