/**
 * @file stationmanager.cpp
 * Implements the StationManager class for managing a collection of stations.
 */

#include "../inlcude/utils/StationManager.h"

using namespace std;

/**
 * @brief Constructs a new Station Manager object.
 * Initializes the station manager with a given number of stations.
 * @param assets Number of stations to be initialized.
 */
StationManager::StationManager(int assets) : assets(assets)
{
    for (int i = 0; i < assets; i++)
    {
        stations.emplace_back(make_shared<Station>(i));
    }
}

/**
 * @brief Retrieves a station by ID.
 * Searches for a station with the specified ID and returns it if found.
 * @param id ID of the station to retrieve.
 * @return shared_ptr<Station> Shared pointer to the requested station, or nullptr if not found.
 */
shared_ptr<Station> StationManager::GetStation(int id)
{
    for (auto &station : stations)
    {
        if (station->GetID() == id)
        {
            return station;
        }
    }
    return nullptr; // Return nullptr if station not found
}

/**
 * @brief Gets the size of the queue at a specific station.
 * @param id ID of the station.
 * @return size_t Size of the station's queue.
 */
size_t StationManager::GetStationSize(int id)
{
    return GetStation(id)->size(); // Assumes GetStation handles nullptr return
}

/**
 * @brief Adds an ID to the queue of the station with the shortest queue.
 * @param id ID to be added.
 * @return int ID of the station to which the ID was added.
 */
int StationManager::AddToShortestQueue(int id)
{
    shared_ptr<Station> targetStation = nullptr;
    size_t minSize = numeric_limits<size_t>::max();

    for (auto &station : stations)
    {
        size_t currentSize = station->size();
        if (currentSize < minSize)
        {
            minSize = currentSize;
            targetStation = station;
        }
    }

    if (targetStation != nullptr)
    {
        targetStation->add(id);
    }

    return targetStation->GetID();
}

/**
 * @brief Removes the front ID from the queue of a specific station.
 * @param id ID of the station from which to pop the queue.
 */
void StationManager::PopStationQueue(int id)
{
    GetStation(id)->remove();
}

/**
 * @brief Returns the amount of station assets in the station manager.
 * @return Number of stations in manager.
 */
int StationManager::GetAssets() const
{
    return assets;
}
