#include "inlcude/main.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <number_of_miners> <number_of_stations>" << std::endl;
        return 1;
    }

    int numberOfMiners = std::atoi(argv[1]);
    int numberOfStations = std::atoi(argv[2]);

    MinerManager mm(numberOfMiners);
    StationManager sm(numberOfStations);

    // Create a TickHandler instance to manage the Miner's ticks
    TickHandler tickHandler(mm, sm, TICK_RATE); // Trigger every 10 milliseconds

    // Start the TickHandler
    tickHandler.start();

    cout << "Building Simulation";
    for (int i = 0; i < 10; ++i)
    { // Loop for a certain number of iterations
        cout << ".";
        if (i % 3 == 2)
        {                                   
            cout << "\b\b\b   \b\b\b";
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << endl;

    tickHandler.stop();

    MetricsHandler::GetInstance().ListAllMetrics();

    return 0;
}
