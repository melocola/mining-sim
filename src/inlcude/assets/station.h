#ifndef STATION_H
#define STATION_H

#include <mutex>
#include <queue>

class Station
{
private:
    std::queue<int> idQueue;
    int stationID;
    mutable std::mutex queueMutex;

public:
    Station(int stationID);
    void add(int id);
    void remove();
    bool isEmpty() const;
    size_t size() const;
    bool isFront(int id) const;
    int GetID() const;
    void SetID(int stationID);
};

#endif // STATION_H
