#ifndef MINER_H
#define MINER_H

#include <iostream>
#include <random>
#include <mutex>

class Miner
{
    public:
        enum STATES
        {
            MINING,
            SEARCHING,
            RETURN,
            WAITING,
            UNLOADING
        };

        enum STATUS
        {
            IDLE,
            FRONT,
            QUEUED,
            READY,
            COMPLETE
        };

        Miner();

        // Getters
        int GetTime() const;
        int GetMiningTime() const;
        int GetState() const;
        int GetQueueStatus() const;
        int GetStationID() const;
        double GetLoad() const;

        // Setters
        void SetTime(int time);
        void SetMiningTime();
        void SetState(int state);
        void SetQueueStatus(int queueStatus);
        void SetStation(int id);
        void SetLoad(double load);

        void tick();

    private:
        int time;
        int miningTime;
        int state;
        int queueStatus;
        int currStation;
        double load;
        void entry();

};


#endif // MINER_H
