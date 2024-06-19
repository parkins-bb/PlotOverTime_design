#include <iostream>
#include <vector>
#include "ustrXdmfIO.hpp"
#include <mpi.h>

int main(int argc, char** argv) {
    // 初始化MPI
    MPI_Init(&argc, &argv);

    // 确保使用Intel oneAPI初始化
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // xmf文件路径和文件名
    std::string xmfFilePath = "";
    std::string xmfFileName = "";

    // 创建Communicator对象
    HSF::Communicator* comm = new HSF::Communicator(MPI_COMM_WORLD);

    // 初始化UstrXdmfTool对象
    HSF::IO::UstrXdmfTool xdmfTool(comm, xmfFilePath, "r");

    // 读取网格拓扑信息
    std::vector<HSF::llabel> cell_data;
    std::vector<HSF::label> cell_offsets;
    std::vector<int> cellTypes;
    llabel numCells = xdmfTool.readTopology(cell_data, cell_offsets, cellTypes, "internalMesh");

    if (rank == 0) {
        std::cout << "Number of nodes: " << numCells << std::endl;
    }

    // 打印拓扑信息
    std::cout << "Loaded topology data from XDMF file. " << std::endl;
    std::cout << "Cell data size: " << cell_data.size() << std::endl;
    std::cout << "Cell offsets size: " << cell_offsets.size() << std::endl;
    std::cout << "Cell types size: " << cellTypes.size() << std::endl;

    // 读取几何数据
    std::vector<HSF::Tensor1<double>> coords;
    xdmfTool.readGeometry(coords, "internalMesh");

    if (rank == 0) {
        std::cout << "Number of nodes: " << numNodes << std::endl;
    }

    // 打印几何信息
    std::cout << "Loaded geometry data from XDMF file. " << std::endl;
    std::cout << "Coordinates size: " << coords.size() << std::endl;

    // 读取场数据（假设场名为"UUU"）
    std::vector<double> fieldData;
    xdmfTool.readAttribute(fieldData, girdName, "UUU");

    if (rank == 0) {
        std::cout << "UUU fieldData: " << fieldData.size() << std::endl;
    }

    // 打印场数据
    std::cout << "Loaded field data 'UUU' from HDF5 file. " << std::endl;
    std::cout << "Field data size: " << fieldData.size() << std::endl;

    // 清理communicator对象资源
    delete comm;

    // 关闭MPI
    MPI_Finalize();

    return 0;
}
