#ifndef PLOT_OVER_TIME_HPP
#define PLOT_OVER_TIME_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "subSet.hpp"
#include "hsfRegion.hpp"
#include "h5part"

class PlotOverTime{
// 该类只有一个实例
public:
    // 输出目录名，Region中可以获得网格及场数据
	PlotOverTime(const string &dump_directory_name, HSF::Region* reg);

    virtual ~PlotOverTime();

    /*
     * @brief 按模拟步注册点
     * @param id: 点/线/面/体编号; type: 点/线/面/体类型; time_step: 模拟的时间步
     * @return 用于csv输出每个时间步的结果（每个点一个文件）
     * */
    void registerByStep(int id, const SetType type, int time_step);

    /*
     * @brief 按照模拟时间注册点
     * @param id: 点/线/面/体编号; type: 点/线/面/体类型; time: 模拟的时间
     * @return 用于csv输出每个时刻的结果（每个点一个文件）
     * */
    void registerByTime(int id, const SetType type, double time);

    /*
     * @brief 注册场变量（默认Region中所有相关场），要求每个注册过的id都要调用该函数
     * */
    void registerPlotFields(const int id, const SetType type);

    /*
     * @brief 相对于上面的接口，增加了field_ids用于指定输出的场变量（注意场的id对应的场类型要与type对应）
     * */
    void registerPlotFields(const int id, const SetType type, std::vector<int> &field_ids);

    /*
     * @brief 
     * */
    template<typename T>
    void registerVariable(const int id, const HSF::SetType type, const std::string &VariableName, const HSF::Coefficient<T>& f);

    /*
     * @brief 输出曲线量到缓存
     * @param time_step: 输出时的时间步; simulation_time: 输出时的物理时刻 
     * */
    void writeData(int time_step, scalar simulation_time);

    /*
     * @brief 将缓存的曲线量输出到数据文件
     * */
    void flushData();

private:
    std::string dumpDirectoryName;
    HSF::Region* region;
    std::map<int, std::pair<HSF::SetType, int>> registeredSteps;  // 存储注册信息
    std::map<int, std::ofstream> outputFiles;  // 存储打开的文件
    std::map<std::pair<int, HSF::SetType>, std::vector<int>> registeredFields_;  // 存储[<id, SetType>, 场变量索引]
    std::map<std::pair<int, HSF::SetType>, std::vector<std::string>> registeredFieldNames_;   // 存储[<id, SetType>, 场名]
    std::map<std::pair<int, HSF::SetType>, std::vector<std::vector<double>>> fieldData_;  // 存储[<id, SetType>, 场数据]

    void openOutputFile(int id, const std::string &type);
    std::string toString(HSF::SetType type);
};

#endif // PLOT_OVER_TIME_HPP
