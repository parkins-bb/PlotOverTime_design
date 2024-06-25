#include "ustrXdmfIO.hpp"
#include "hdf5Tool.hpp"
#include "subSet.hpp"
#include "region.hpp"
#include "ustrMesh.hpp"
#include <iostream>

// 初始化和加载数据
void initializeAndLoadData(const std::string& xmfFileName, const std::string& hdf5FileName, HSF::IO::UstrXdmfTool& xdmfIO, HSF::IO::Hdf5Tool& hdf5IO, Communicator* comm){
    xdmfIO.reset(comm, xmfFileName);
    hdf5IO.reset(hdf5FileName);

    xdmfIO.loadXmfFile();
    hdf5IO.loadHdf5File();
}

// 解析数据
void parseData(HSF::IO::UstrXdmfTool& xdmfIO, HSF::IO::Hdf5Tool& hdf5IO, int set_id, HSF::Region& region, HSF::SubSet& subset) {
    // 从XMF文件中解析几何信息和拓扑结构
    std::vector<label> cell_data;
    std::vector<label> cell_offsets;
    std::vector<int> cellTypes;
    std::string gridName = " "; // 暂时没有加入

    xdmfIO.readTopology(cell_data, cell_offsets, cellTypes, gridName);

    // 从HDF5文件中读取所有场信息
    std::vector<double> fieldData;
    std::string fieldName = " "; // 暂时没有加入

    hdf5IO.readFieldData(fieldData, gridName, fieldName);

    // 处理子集信息
    subset.set(cell_data, cell_offsets, cellTypes);

    // 解析场信息
    region.setFieldData(fieldData);
}

// 输出结果
void outputResults(const HSF::Region& region, const HSF::SubSet& subset) {
    // 输出集合上的所有场信息
    for (const auto& field : region.getFields()) {
        std::cout << "Field Name: " << field.name << std::endl;
        std::cout << "Data: ";
        for (const auto& value : field.data) {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }

    // 输出定场信息
    for (const auto& field : subset.getFields()) {
        std::cout << "Subset Field Name: " << field.name << std::endl;
        std::cout << "Data: ";
        for (const auto& value : field.data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// 加载xmf和h5文件进行测试
int main(int argc, char** argv) {
    // 加载xmf文件和对应的h5文件
    std::string xmfFileName = "../../data/forward_step_2d_X1E4_100.xmf";
    std::string hdf5FileName = "../../data/forward_step_2d_X1E4_100.h5";

    // 初始化Communicator，这个功能暂时未知
    Communicator* comm = new Communicator();

    // 初始化ustrXdmfIO 和 hdf5Tool 模块
    HSF::IO::UstrXdmfTool xdmfIO(comm, xmfFileName);
    HSF::IO::Hdf5Tool hdf5IO(hdf5FileName);

    // 加载数据
    initializeandLoadData(xmfFileName, hdf5FileName, xdmfIO, hdf5IO, comm);

    // 初始化Region 和 SubSet
    HSF::Region region;
    HSF::SubSet subset;

    // 解析数据
    int set_id = 1;  // 等会需要一个个测试
    parseData(xdmfIO, hdf5IO, set_id, region, subset);

    // 输出结果
    outputResults(region, subset);

    return 0;
}
