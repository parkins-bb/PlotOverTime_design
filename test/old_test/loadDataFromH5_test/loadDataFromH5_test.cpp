#include "PlotOverTime.hpp"
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    Communicator* comm = new Communicator(MPI_COMM_WORLD);

    std::string xmfFileName = "path/to/your.xmf";

    PlotOverTime plotOverTime(comm, xmfFileName);

    auto setIdPaths = plotOverTime.parseXmfFile();

    for (const auto& entry : setIdPaths) {
        std::cout << "set_id: " << entry.first << std::endl;
        for (const auto& path : entry.second) {
            std::cout << "  Path: " << path << std::endl;
        }

        auto datasetPaths = plotOverTime.getDatasetPaths(entry.first);
        for (const auto& dataset : datasetPaths) {
            std::cout << "  " << dataset.first << ": " << dataset.second << std::endl;
        }

        plotOverTime.loadDataFromH5(entry.first);

        std::cout << "Loaded data for set_id: " << entry.first << std::endl;
    }

    delete comm;
    MPI_Finalize();

    return 0;
}

