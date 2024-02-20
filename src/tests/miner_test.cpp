#include <gtest/gtest.h>
#include "../inlcude/assets/miner.h"

class MinerTest : public ::testing::Test
{
    public:
        Miner miner;
};

TEST_F(MinerTest, TestDefaultConstructor)
{
    Miner miner; // Instantiate the class under test
    EXPECT_EQ(miner.GetTime(), 0);
    EXPECT_EQ(miner.GetMiningTime(), 0);
    EXPECT_EQ(miner.GetState(), Miner::STATES::WAITING);
    EXPECT_EQ(miner.GetQueueStatus(), Miner::STATUS::IDLE);
    EXPECT_EQ(miner.GetStationID(), 0);
    EXPECT_EQ(miner.GetLoad(), 0.0);
}

TEST_F(MinerTest, TestGettersAfterConstruction)
{
    // Default values after construction
    EXPECT_EQ(miner.GetTime(), 0);
    EXPECT_EQ(miner.GetMiningTime(), 0);
    EXPECT_EQ(miner.GetState(), Miner::STATES::MINING);
    EXPECT_EQ(miner.GetQueueStatus(), Miner::STATUS::IDLE);
    EXPECT_EQ(miner.GetStationID(), 0);
    EXPECT_EQ(miner.GetLoad(), 0.0);
}

TEST_F(MinerTest, TestSettersAndGetters)
{
    // Test setting and getting time
    miner.SetTime(10);
    EXPECT_EQ(miner.GetTime(), 10);
    miner.SetMiningTime();
    EXPECT_EQ(miner.GetMiningTime(), 10);
    miner.SetState(Miner::STATES::RETURN);
    EXPECT_EQ(miner.GetState(), Miner::STATES::RETURN);
    miner.SetQueueStatus(Miner::STATUS::QUEUED);
    EXPECT_EQ(miner.GetQueueStatus(), Miner::STATUS::QUEUED);
    miner.SetStation(3);
    EXPECT_EQ(miner.GetStationID(), 3);
    miner.SetLoad(0.5);
    EXPECT_EQ(miner.GetMiningTime(), 0.5);
}

TEST_F(MinerTest, TestStateTransitions)
{
    // Test state transition
    miner.SetState(Miner::STATES::SEARCHING);
    miner.tick();
    EXPECT_EQ(miner.GetState(), Miner::STATES::SEARCHING);
    miner.SetQueueStatus(Miner::STATUS::FRONT);
    miner.tick();
    EXPECT_EQ(miner.GetState(), Miner::STATES::RETURN);
}