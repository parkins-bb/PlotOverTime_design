#include "PlotOverTime.hpp"
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    Communicator* comm = new Communicator(MPI_COMM_WORLD);

    std::string xmfFileName = "path/to/your.xmf";

    PlotOverTime plotOverTime(comm, xmfFileName);

    auto setIdPaths = plotOverTime.parseXmfFile();

    for (const auto& entry : setIdPaths) {
        int setId = entry.first;
        std::cout << "Loading data for set_id: " << setId << std::endl;
        plotOverTime.loadDataFromH5(setId);

        std::string outputFileName = "output_" + std::to_string(setId) + ".h5";
        std::cout << "Writing data to: " << outputFileName << std::endl;
        plotOverTime.writeDataToHDF5(setId, outputFileName);
    }

    delete comm;
    MPI_Finalize();

    return 0;
}

