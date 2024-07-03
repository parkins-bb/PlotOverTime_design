#ifndef PLOT_OVER_TIME_HPP
#define PLOT_OVER_TIME_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <cassert>
#include <iomanip>
#include "subSet.hpp"
#include "hsfRegion.hpp"
#include "hsfField.hpp"
#include "ustrMesh.hpp"

namespace HSF {

class PlotOverTime{
// 该类只有一个实例
public:
    /* 
     * @brief 输出目录名，Region中可以获得网格及场数据 
     * @param dump_directory_name: 输出数据文件的目录名称; reg: 指向网格区域对象的指针
     * @return 初始化dumpDirectoryName和Region变量
     * */
	PlotOverTime(const std::string &dump_directory_name, HSF::Region* reg);

    virtual ~PlotOverTime();

    /*
     * @brief 注册特定时间步输出的数据
     * @param id: 点/线/面/体编号; type: 点/线/面/体类型; time_step: 模拟的时间步
     * @return 用于csv输出每个时间步的结果（每个点一个文件）
     * */
    void registerByStep(int id, const SetType type, int time_step);

    /* 暂未实现
     * @brief 注册特定时间输出的数据
     * @param id: 点/线/面/体编号; type: 点/线/面/体类型; time: 模拟的时间步
     * @return 用于csv输出每个时刻的结果（每个点一个文件）
     * */
    
    void registerByTime(int id, const SetType type, int time_step);
    

    /*
     * @brief 注册场变量（默认Region中所有相关场），要求每个注册过的id都要调用该函数
     * @param id: 点/线/面/体的编号; type: 点/线/面/体的类型(HSF::SetType枚举)
     * */
    void registerPlotFields(const int id, const SetType type);

    /*
     * @brief 相对于上面的接口，增加了field_ids用于指定输出的场变量（注意场的id对应的场类型要与type对应）
     * @param id: 点/线/面/体的编号; type: 点/线/面/体的类型(HSF::SetType枚举)
     * @param field_ids: 需要输出的场变量的索引列表
     * @return 注册指定id和SetType的特定场变量
     * */
    void registerPlotFields(const int id, const SetType type, std::vector<int> &field_ids);

    /* 暂未实现
     * @brief 
     * */
    template<typename T>
    void registerVariable(const int id, const HSF::SetType type, const std::string &VariableName, const HSF::Coefficient<T>& f);

    /* 不需要实现
     * @brief 此函数为了获取节点的坐标值，将坐标值写入CSV文件，并存储在
     * @note 节点的坐标值是实时更新的
     * @param id:节点的坐标; time_step:时间步
     * */
    void outputNodeCoordinates(int id, int time_step);

    /*
     * @brief 输出曲线量到缓存
     * @param time_step: 输出时的时间步; simulation_time: 输出时的物理时刻 
     * */
    void writeData(int time_step);

    /*
     * @brief 将缓存的曲线量输出到数据文件
     * */
    void flushData();

private:
    std::string dumpDirectoryName;  // 存储输出数据文件的目录名称
    HSF::Region* region;  // 指向网格区域对象，用于获取场数据
    // 存储注册在特定时间步输出的点/线/面/体的数据。键是时间步，值是一个包含ID和SetType的对
    std::map<int, std::pair<HSF::SetType, int>> registeredSteps;
    // 存储打开的输出文件流，键是点/线/面/体的ID，值是对应的输出文件流
    std::map<int, std::ofstream> outputFiles;
    // 存储注册的场变量，键是ID和SetType的对，值是场变量的索引列表
    std::map<std::pair<int, HSF::SetType>, std::vector<int>> registeredFields_;  
    std::map<std::pair<int, HSF::SetType>, std::vector<std::string>> registeredFieldNames_;   // 存储[<id, SetType>, 场名]
    std::map<std::pair<int, HSF::SetType>, std::vector<std::vector<double>>> fieldData_;  // 存储[<id, SetType>, 场数据]

    /*
     * @brief 获取输出文件流，如果文件未打开则创建新的文件
     * @param id: 点/线/面/体的编号; type: 点/线/面/体的类型(HSF::SetType枚举)
     * @return 输出对应ID和SetType的输出文件流
     * */
    std::ofstream& getFile(int id, HSF::SetType type);

    void openOutputFile(int id, const std::string &type);
    
    /*
     * @brief 将HSF::SetType枚举值转换为对应的字符串表示
     * @param type: 点/线/面/体的类型(HSF::SetType枚举)
     * @return 类型的字符串表示
     * */
    std::string toString(HSF::SetType type);
};

} // namespace HSF

#endif // PLOT_OVER_TIME_HPP
