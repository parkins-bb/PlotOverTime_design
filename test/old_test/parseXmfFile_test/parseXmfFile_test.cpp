#include "PlotOverTime.hpp"
#include <mpi.h>

int main(int argc, char* argv[]) {
    // 初始化MPI
    MPI_Init(&argc, &argv);

    // 初始化 Communicator
    Communicator* comm = new Communicator(MPI_COMM_WORLD);

    // 示例 XMF 文件路径
    std::string xmfFileName = "path/to/your.xmf";

    // 创建 PlotOverTime 对象
    PlotOverTime plotOverTime(comm, xmfFileName);

    // 解析 XMF 文件，获取 set_id 对应的节点路径
    auto setIdPaths = plotOverTime.parseXmfFile();

    // 输出解析结果
    for (const auto& entry : setIdPaths) {
        std::cout << "set_id: " << entry.first << std::endl;
        for (const auto& path : entry.second) {
            std::cout << "  Path: " << path << std::endl;
        }
    }

    // 释放资源
    delete comm;

    // 结束MPI
    MPI_Finalize();

    return 0;
}

