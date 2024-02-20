#ifndef MINER_MANAGER_H
#define MINER_MANAGER_H

#include "../assets/miner.h"
#include <map>
#include <utility>

class MinerManager
{
    public:
        MinerManager(int assets);
        Miner& GetMiner(int id);
        int GetAssets();

    private:
        std::map<int, Miner> miners;
        int assets;
};

#endif // MINER_MANAGER_H
