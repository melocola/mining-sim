/**
 * @file metricshandler.h
 * @brief Defines the MetricsHandler class for managing and recording metrics.
 */
#include "../inlcude/utils/MetricsHandler.h"

using namespace std;

/**
 * @brief Retrieves the singleton instance of MetricsHandler.
 * @return MetricsHandler& Reference to the singleton instance.
 */
MetricsHandler &MetricsHandler::GetInstance()
{
    static MetricsHandler instance;
    return instance;
}

/**
 * @brief Records a metric under a specified category and metric name.
 * @param category The category under which to record the metric.
 * @param metricName The name of the metric to record.
 * @param value The value of the metric.
 */
void MetricsHandler::RecordMetric(const string &category, const string &metricName, double value)
{
    metrics[category][metricName].push_back(value);
}

/**
 * @brief Retrieves metrics for a specified category.
 * @param category The category for which to retrieve metrics.
 * @return map<string, vector<double>> A map of metric names to their recorded values.
 */
map<string, vector<double>> MetricsHandler::GetMetrics(const string &category) const
{
    auto it = metrics.find(category);
    if (it != metrics.end())
    {
        return it->second;
    }
    return {};
}

/**
 * @brief Lists all metrics recorded, formatted for console output.
 * @note metrics is a std::map<std::string, std::map<std::string, std::vector<double>>>
 */
void MetricsHandler::ListAllMetrics()
{
    //We create this tuple to store the averages/totals/max/min we calculate so we can add them to the map before parsing to json
    vector<tuple<string, string, double>> finalMetrics;
    //Entering the Category/First layer of this map of maps. It's the Miner or Station we're logging
    for (const auto &categoryPair : metrics)
    {
        if (categoryPair.first.rfind("Miner", 0) == 0)
        {
            cout << "Category: " << categoryPair.first << endl;
            // Entering the Data/Second Layer. These are the stats like DistanceTraveled and the list/array of values over the time of the simulation
            for (const auto &metricPair : categoryPair.second)
            {
                double total = accumulate(metricPair.second.begin(), metricPair.second.end(), 0.0);
                double average = metricPair.second.empty() ? 0.0 : total / metricPair.second.size();
                auto [minIt, maxIt] = minmax_element(metricPair.second.begin(), metricPair.second.end());
                
                cout << "  " << metricPair.first
                            << " - Total: " << total
                            << ", Average: " << average
                            << ", Max: " << (maxIt != metricPair.second.end() ? *maxIt : 0.0)
                            << ", Min: " << (minIt != metricPair.second.end() ? *minIt : 0.0)
                            << endl;
                finalMetrics.push_back(make_tuple(categoryPair.first, metricPair.first+"-Total", total));
                finalMetrics.push_back(make_tuple(categoryPair.first, metricPair.first + "-Avg", average));
                finalMetrics.push_back(make_tuple(categoryPair.first, metricPair.first + "-Max", *maxIt));
                finalMetrics.push_back(make_tuple(categoryPair.first, metricPair.first + "-Min", *minIt));
            }
        }
        //Same as Miner but we only need the Average of material quality and the total material volume
        else if (categoryPair.first.rfind("Station", 0) == 0)
        {
            cout << "Category: " << categoryPair.first << endl;
            for (const auto &metricPair : categoryPair.second)
            {
                auto category = categoryPair.first;
                if (metricPair.first == "MaterialVolume" || metricPair.first == "UtilizationRate")
                {
                    double total = accumulate(metricPair.second.begin(), metricPair.second.end(), 0.0);
                    cout << "  " << metricPair.first << " - Total: " << total << endl;
                    finalMetrics.push_back(make_tuple(categoryPair.first, metricPair.first + "Total", total));
                }
                else if (metricPair.first == "MaterialQuality")
                {
                    double total = 0.0;
                    size_t count = 0;
                    for (double value : metricPair.second)
                    {
                        if (value > 0)
                        {
                            total += value;
                            ++count;
                        }
                    }
                    double average = count > 0 ? total / count : 0.0;
                    cout << "  " << metricPair.first << " - Average: " << average << endl;
                    finalMetrics.push_back(make_tuple(categoryPair.first, metricPair.first + "Avg", average));
                }
            }
        }
        cout << endl;
    }

    //Transfers the calculated metrics to the map
    for(auto metric : finalMetrics)
    {
        double value = get<2>(metric);
        RecordMetric(get<0>(metric), get<1>(metric), value);
    }
    SaveMetricsToJson("test.json");
}
/**
 * @brief Exports all the granular data to a json file
 */
void MetricsHandler::SaveMetricsToJson(const string &filename) const
{
    // Getting timestamp for file name
    auto now = chrono::system_clock::now();
    auto now_time_t = chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&now_time_t), "%Y-%m-%d_%H-%M-%S");

    // Construct filename with timestamp
    string timestampedFilename = filename.substr(0, filename.find_last_of('.')) + "_" + ss.str() + filename.substr(filename.find_last_of('.'));

    // Construct the path relative to the executable location
    string relativePath = "./" + timestampedFilename; // Adjusts path to point to src directory

    //Parsing the metrics to a json file
    nlohmann::json json;
    for (const auto &cat : metrics)
    {
        for (const auto &met : cat.second)
        {
            json[cat.first][met.first] = met.second;
        }
    }

    //Check to see if the file was made properly, if so, generate our JSON
    ofstream file(relativePath); // Use the constructed path
    if (file.is_open())
    {
        file << json.dump(4); // Pretty printing with 4 spaces indent
        file.close();
        cout << "Metrics saved to " << relativePath << endl;
    }
    else
    {
        cout << "Unable to open file: " << relativePath << endl;
    }
}
