/**
 * @file minermanager.cpp
 * Implementation of the MinerManager class, managing miners in the simulation.
 */

#include "../inlcude/utils/minermanager.h"

using namespace std;

/**
 * @brief Constructs a new Miner Manager object.
 *
 * Initializes the miner manager with a specific number of miners. Each miner is
 * instantiated and stored in a map with their ID as the key.
 *
 * @param assets The number of miners to manage.
 */
MinerManager::MinerManager(int assets) : assets(assets)
{
    for (int i = 0; i < assets; i++)
    {
        miners.emplace(make_pair(i, Miner()));
    }
}

/**
 * @brief Retrieves a reference to a miner by their ID.
 *
 * Searches the collection of miners for the one with the specified ID. If the miner
 * is found, returns a reference to it. Throws an exception if no miner with the
 * given ID exists.
 *
 * @param id The ID of the miner to retrieve.
 * @return Miner& A reference to the requested miner.
 * @throws std::out_of_range if a miner with the specified ID does not exist.
 */
Miner &MinerManager::GetMiner(int id)
{
    auto miner = miners.find(id);
    if (miner != miners.end())
    {
        return miner->second;
    }
    else
    {
        throw std::out_of_range("Invalid miner ID");
    }
}

/**
 * @brief Gets the total number of miners managed by this manager.
 *
 * @return int The number of miners.
 */
int MinerManager::GetAssets()
{
    return assets;
}
