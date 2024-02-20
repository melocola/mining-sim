#ifndef METRICSHANDLER_H
#define METRICSHANDLER_H

#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <tuple>

class MetricsHandler
{
    public:
        static MetricsHandler &GetInstance();
        MetricsHandler(const MetricsHandler &) = delete;
        MetricsHandler &operator=(const MetricsHandler &) = delete;
        std::map<std::string, std::vector<double>> GetMetrics(const std::string &category) const;
        void ListAllMetrics();
        void SaveMetricsToJson(const std::string &filename) const;
        void RecordMetric(const std::string &category, const std::string &metricName, double value);

    private :
        MetricsHandler() = default;
        std::map<std::string, std::map<std::string, std::vector<double>>> metrics;
};

#endif // METRICSHANDLER_H
