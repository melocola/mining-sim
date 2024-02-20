/**
 * @file miner.cpp
 * @brief Implementation of the Miner class.
 *
 * This file implements the functionalities of the Miner class, which includes managing
 * the state, queue status, and other properties of a miner in the simulation.
 */

#include "../inlcude/assets/miner.h"

using namespace std;

/**
 * @brief Constructor initializing a miner with default properties.
 */
Miner::Miner() : time(0), state(MINING), queueStatus(0), currStation(-1), load(-1)
{
    entry();
}

/**
 * @brief Returns the current time for the miner's operation.
 * @return int Current time.
 */
int Miner::GetTime() const
{
    return this->time;
}

/**
 * @brief Returns the mining time.
 * @return int Mining time.
 */
int Miner::GetMiningTime() const
{
    return this->miningTime;
}

/**
 * @brief Returns the current state of the miner.
 * @return int Current state.
 */
int Miner::GetState() const
{
    return state;
}

/**
 * @brief Returns the queue status of the miner.
 * @return int Queue status.
 */
int Miner::GetQueueStatus() const
{
    return queueStatus;
}

/**
 * @brief Returns the current station ID associated with the miner.
 * @return int Current station ID.
 */
int Miner::GetStationID() const
{
    return currStation;
}

/**
 * @brief Returns the current miner load.
 * @return Load Percentage from site.
 */
double Miner::GetLoad() const
{
    return load;
}

/**
 * @brief Sets the time for the miner's operation.
 * @param time New operation time.
 */
void Miner::SetTime(int time)
{
    this->time = time;
}

/**
 * @brief Sets the mining time to the current time. This function is only called on entry of the Mining State.
 */
void Miner::SetMiningTime()
{
    miningTime = time;
}

/**
 * @brief Sets the state of the miner.
 * @param state New state.
 */
void Miner::SetState(int state)
{
    this->state = state;
}

/**
 * @brief Sets the queue status of the miner.
 * @param queueStatus New queue status.
 */
void Miner::SetQueueStatus(int queueStatus)
{
    this->queueStatus = queueStatus;
}

/**
 * @brief Sets the current station ID for the miner.
 * @param currStation New station ID.
 */
void Miner::SetStation(int currStation)
{
    this->currStation = currStation;
}

/**
 * @brief Sets the current load for the miner.
 * @param load New miner load percentage.
 */
void Miner::SetLoad(double load)
{
    this->load = load;
}

/**
 * @brief Entry logic for the miner's state, handling initial actions based on the current state.
 */
void Miner::entry()
{
    switch (GetState())
    {
    case STATES::MINING:
        {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distr(12, 60);
            uniform_real_distribution<double> loadCapacity(0.75, 1.0);
            SetTime(distr(gen));
            SetMiningTime();
            SetLoad(loadCapacity(gen));
            SetQueueStatus(IDLE);
        }
        break;
    case STATES::SEARCHING:
        SetTime(0);
        break;
    case STATES::RETURN:
        SetTime(6);
        break;
    case STATES::WAITING:
        SetTime(0);
        break;
    case STATES::UNLOADING:
        SetTime(1);
        break;
    };
}

/**
 * @brief Tick logic for the miner, handling state transitions and actions for each tick.
 */
void Miner::tick()
{
    switch (GetState())
    {
        case MINING:
        {
            int curTimeReturn = GetTime(); // Unique variable name
            if (curTimeReturn > 0)
            {
                SetTime(curTimeReturn - 1);
            }
            else
            {
                SetState(SEARCHING);
                entry();
            }
            break;
        }
        case SEARCHING:
        {
            int qs = GetQueueStatus();
            if (qs == FRONT)
            {
                SetState(RETURN);
                entry();
            }
            else if (qs == QUEUED)
            {
                SetState(WAITING);
                entry();
            }
            break;
        }
        case RETURN:
        {
            int curTimeReturn = GetTime(); // Unique variable name
            if (curTimeReturn > 0)
                SetTime(curTimeReturn - 1);
            else
            {
                SetState(UNLOADING);
                entry();
            }
            break;
        }
        case WAITING:
        {
            int qs = GetQueueStatus();
            if (qs == READY)
            {
                SetState(RETURN);
                entry();
            }
            break;
        }
        case UNLOADING:
        {
            int curTimeUnloading = GetTime();
            if (curTimeUnloading > 0)
            {
                SetTime(curTimeUnloading - 1);
                SetQueueStatus(COMPLETE);
            }
            else
            {   
                SetState(MINING);
                entry();
            }
            break;
        }
    };
}