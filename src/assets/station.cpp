/**
 * @file station.cpp
 * @brief Implementation of the Station class.
 *
 * The Station class represents a station in the mining simulation. It manages
 * a queue of miners waiting to be processed, providing thread-safe operations
 * to add and remove miners from the queue, check queue status, and manage station IDs.
 */

#include "../inlcude/assets/station.h"

using namespace std;

/**
 * @brief Constructs a Station object with a specific station ID.
 * @param stationID Unique identifier for the station.
 */
Station::Station(int stationID) : stationID(stationID) {}

/**
 * @brief Adds a miner ID to the station's queue.
 * @param id Miner ID to add to the queue.
 */
void Station::add(int id)
{
    lock_guard<mutex> lock(queueMutex);
    idQueue.push(id);
}

/**
 * @brief Removes the front miner ID from the station's queue.
 */
void Station::remove()
{
    lock_guard<mutex> lock(queueMutex);
    if (!idQueue.empty())
    {
        idQueue.pop();
    }
}

/**
 * @brief Checks if the station's queue is empty.
 * @return true If the queue is empty, false If the queue contains one or more IDs.
 */
bool Station::isEmpty() const
{
    lock_guard<mutex> lock(queueMutex);
    return idQueue.empty();
}

/**
 * @brief Gets the size of the station's queue.
 * @return size_t The number of IDs in the queue.
 */
size_t Station::size() const
{
    lock_guard<mutex> lock(queueMutex);
    return idQueue.size();
}

/**
 * @brief Checks if a specific miner ID is at the front of the queue.
 * @param id Miner ID to check.
 * @return true If the specified ID is at the front of the queue. False Otherwise.
 */
bool Station::isFront(int id) const
{
    lock_guard<mutex> lock(queueMutex);
    return !idQueue.empty() && idQueue.front() == id;
}

/**
 * @brief Gets the station's ID.
 * @return int The station's ID.
 */
int Station::GetID() const
{
    return stationID;
}

/**
 * @brief Sets the station's ID.
 * @param stationID The new ID for the station.
 */
void Station::SetID(int stationID)
{
    this->stationID = stationID;
}
