#ifndef PLOT_OVER_TIME_HPP
#define PLOT_OVER_TIME_HPP

#include "ustrXdmfIO.hpp"
#include "pugixml.hpp"
#include "hdf5Tool.hpp"
#include "utilityCommunicator.hpp"
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class PlotOverTime {
    public:
        /* 
         * @brief 构造函数，初始化XdmfTool对象并解析XMF文件
         * @param Communicator对象和XMF文件名（文件路径）
         * @return 初始化xdmfIO_和解析setIdPaths_
         * */
        PlotOverTime(HSF::IO::UstrXdmfTool* xdmf_tool, UTILITY::Communicator* comm, const std::string& xmfFileName);

        /*
         * @brief 析构函数
         * */
        ~PlotOverTime();

        /* 
         * @brief 解析XMF文件，提取每个set_id的节点路径
         * @param 无（使用成员变量xdmfIO_解析XMF文件）
         * @return 返回一个包含每个set_id和对应节点路径的映射
         * */
        std::map<int, std::vector<std::string>> parseXmfFile();

        /*
         * @brief 获取指定set_id的几何、拓扑、属性和物理场数据路径
         * @param 输入set_id
         * @return 返回一个包含数据集名称和对应HDF5文件路径的映射
         * */
        std::map<std::string, std::string> getDatasetPaths(int setId);

        /* 
         * @brief 加载set_id对应h5文件的数据集
         * @param 输入set_id
         * @return 将数据存储在setIdData_成员变量中
         * */
        void loadDataFromH5(int setId);
        
        /* 
         * @brief 将指定set_id的所有数据写入HDF5文件
         * @param 输入set_id和输出文件名
         * @return 将数据写入指定的HDF5文件
         * */
        void writeDataToHDF5(int setId, const std::string& outputFileName);

        /*
         * @brief 将指定的set_id和指定字段的数据写入HDF5文件
         * @param 输入set_id，字段名称和输出文件名
         * @return 将指定字段的数据写入指定的HDF5文件
         * */
        void writeSpecifiedFieldToHDF5(int setId, const std::string& fieldName, const std::string& outputFileName);

    private:
        HSF::IO::XdmfTool xdmfIO_;  // XdmfTool对象，用于处理XMF文件的解析和操作
        std::map<int, std::vector<std::string>> setIdPaths_;  // 存储每个set_id对应的节点路径
        std::map<int, std::map<std::string, std::vector<double>>> setIdData_;  // 用于存储从h5文件中读取的每个set_id数据
};

#endif // PLOT_OVER_TIME_HPP
