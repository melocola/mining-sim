#ifndef STATIONMANAGER_H
#define STATIONMANAGER_H

#include "../assets/station.h"
#include <vector>
#include <mutex>
#include <algorithm>
#include <limits>
#include <utility>
#include <iostream>
#include <memory>

class StationManager
{
public:
    StationManager(int assets);
    std::shared_ptr<Station> GetStation(int id);
    size_t GetStationSize(int id);
    int AddToShortestQueue(int id);
    void PopStationQueue(int id);
    int GetAssets() const;

private:
    int assets;
    std::vector <std::shared_ptr < Station >> stations;
    std::mutex stationsMtx;
};

#endif // STATIONMANAGER_H
