#ifndef PLOT_OVER_TIME_HPP
#define PLOT_OVER_TIME_HPP

#include <string>
#include <vector>
#include <map>
#include <typeindex>
#include <iostream>
#include <fstream>
#include "hsfField.hpp"
#include "hsfRegion.hpp"

namespace HSF {

class PlotOverTime {
private:
    std::string dumpDirectoryName;
    Region* region;
    std::map<std::pair<int, SetType>, std::vector<int>> registeredFields_;
    std::map<std::pair<int, SetType>, std::vector<std::string>> registeredFieldNames_;
    std::map<std::pair<int, SetType>, std::vector<std::vector<double>>> fieldData_;
    std::map<int, std::pair<int, SetType>> registeredSteps;

public:
    PlotOverTime(Region* reg, const std::string& directory);
    ~PlotOverTime();

    void registerByStep(int id, const SetType type, int time_step);
    void registerByTime(int id, const SetType type, int time_step);
    void registerPlotFields(int id, const SetType type);
    void registerPlotFields(int id, const SetType type, std::vector<int>& field_ids);
    void writeData(int time_step);
    void flushData();
    std::string toString(SetType type);
};

} // namespace HSF

#endif // PLOT_OVER_TIME_HPP

