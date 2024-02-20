/**
 * @file tickhandler.h
 * @brief TickHandler class manages timed events for miners and stations in the simulation.
 *
 * This class is responsible for managing the lifecycle of miners within the simulation,
 * including their creation, state updates, and interactions with stations. It uses a
 * multi-threaded approach to simulate concurrent actions of different miners.
 */

#include "../inlcude/utils/tickhandler.h"

using namespace std;

/**
 * @brief Constructs a TickHandler with references to the miner and station managers and sets the tick interval.
 * @param minerManager Reference to the miner manager.
 * @param stationManager Reference to the station manager.
 * @param interval_ms Tick interval in milliseconds.
 */
TickHandler::TickHandler(MinerManager &minerManager, StationManager &stationManager, unsigned int interval_ms)
    : minerManager(minerManager), stationManager(stationManager), interval_ms_(interval_ms), keepRunning_(false)
{
}

/**
 * @brief Starts the simulation by launching threads for each mine.
 */
void TickHandler::start()
{
    keepRunning_ = true;
    for (int i = 0; i < minerManager.GetAssets(); i++)
    {
        timerThreads_.emplace_back([this, i]()
                                   { this->tick(i); });
    }
}

/**
 * @brief Stops the simulation and joins all miner threads to ensure clean shutdown.
 */
void TickHandler::stop()
{
    keepRunning_ = false;
    for (auto &thread : timerThreads_)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    timerThreads_.clear();
}

/**
 * @brief Destructor that ensures all miner threads are stopped.
 */
TickHandler::~TickHandler()
{
    stop();
}

/**
 * @brief Manages the tick events for a single miner, handling their state transitions and actions.
 * @param id Unique identifier of the miner to be processed.
 */
void TickHandler::tick(int id)
{
    Miner miner = minerManager.GetMiner(id);
    int tickCount = 0;
    //The max tick is set 864 since each tick is 5 minutes and we want to simulate a run of 72 hours
    while (keepRunning_ && tickCount < MAX_TICK)
    {
        auto start = chrono::steady_clock::now();

        // This is where the miner and station logic meets
        // A miner's state dictates what needs to be done with a station
        switch(miner.GetState())
        {
            // In a searching state, after finding the shortest wait time at a queue, it either heads directly to the station to unload
            // or it enters a waiting state until the station is open for them
            case Miner::SEARCHING:
            {
                auto targetStation = stationManager.GetStation(stationManager.AddToShortestQueue(id));

                if (targetStation && targetStation->isEmpty())
                    miner.SetQueueStatus(Miner::FRONT);
                else
                    miner.SetQueueStatus(Miner::QUEUED);
                miner.SetStation(targetStation->GetID());
                break;
            }
            // A miner is waiting unitil its first in the queue to unload
            case Miner::WAITING:
            {
                auto station = stationManager.GetStation(miner.GetStationID());

                if (station && station->isFront(id))
                    miner.SetQueueStatus(Miner::READY);
                break;
            }
            // While unloading, the miner also submits it status data at this point so we recieve metrics
            case Miner::UNLOADING:
            {
                if(miner.GetQueueStatus() == Miner::COMPLETE)
                {
                    int sID = miner.GetStationID();
                    auto station = stationManager.GetStation(sID);

                    MinierMetrics(miner, id);
                    StationMetrics(*station, sID, miner.GetLoad());
                    stationManager.PopStationQueue(sID);
                }
                break;
            }
        }

        int ticksRemaining = MAX_TICK - tickCount;
        
        //Handles the logic for the minner after station needs are established
        miner.tick();

        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
        auto timeToWait = interval_ms_ - elapsed.count();
        if (timeToWait > 0)
        {
            this_thread::sleep_for(chrono::milliseconds(timeToWait));
        }
        tickCount++;
    }
}

/**
 * @brief Records performance metrics for a miner.
 * @param miner Reference to the miner object.
 * @param id Identifier of the miner.
 */
void TickHandler::MinierMetrics(Miner &miner, int id)
{
    MetricsHandler &metricsHandler = MetricsHandler::GetInstance();
    string asset = "Miner-" + to_string(id + 1);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> fuelConsumptionDistr(0.0, 0.02);
    double ranFuelConsumption = fuelConsumptionDistr(gen);
    
    metricsHandler.RecordMetric(asset, "DistanceTraveled", 1.0);
    metricsHandler.RecordMetric(asset, "LoadCapacityUtilized", miner.GetLoad());
    metricsHandler.RecordMetric(asset, "FuelConsumption", ranFuelConsumption);
    metricsHandler.RecordMetric(asset, "MiningTime", miner.GetMiningTime());
}

/**
 * @brief Records performance metrics for a station.
 * @param station Reference to the station object.
 * @param id Identifier of the station.
 */
void TickHandler::StationMetrics(Station &station, int id, double load)
{
    MetricsHandler &metricsHandler = MetricsHandler::GetInstance();
    string asset = "Station-" + to_string(id + 1);

    metricsHandler.RecordMetric(asset, "QueueTimes", static_cast<double>(station.size()));

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> materialQuality(0.75, 1.0);
    double raMaterialQuality = materialQuality(gen);

    metricsHandler.RecordMetric(asset, "MaterialVolume", load);
    metricsHandler.RecordMetric(asset, "MaterialQuality", raMaterialQuality);
    metricsHandler.RecordMetric(asset, "UtilizationRate", 1);
    
}