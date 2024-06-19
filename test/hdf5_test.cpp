#include <H5Cpp.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char **argv) {
    // 初始化MPI
    MPI_Init(&argc, &argv);

    // 获取进程ID和进程总数
    int mpi_rank, mpi_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    // HDF5文件名
    const char* FILE_NAME = "parallel_example.h5";

    // 创建文件访问属性列表，设置并行访问
    hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    // 创建或打开HDF5文件
    hid_t file_id;
    if (mpi_rank == 0) {
        file_id = H5Fcreate(FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    } else {
        file_id = H5Fopen(FILE_NAME, H5F_ACC_RDWR, plist_id);
    }
	H5Pclose(plist_id);

	// 创建数据空间
    hsize_t dims[2] = {static_cast<hsize_t>(mpi_size), 10};
    hid_t dataspace_id = H5Screate_simple(2, dims, NULL);

    // 创建数据集
    hid_t dataset_id;
    if (mpi_rank == 0) {
        dataset_id = H5Dcreate(file_id, "dataset", H5T_NATIVE_INT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    } else {
        dataset_id = H5Dopen(file_id, "dataset", H5P_DEFAULT);
    }

    // 初始化数据
    int data[10];
    for (int i = 0; i < 10; i++) {
        data[i] = mpi_rank * 10 + i;
    }

    // 设置数据传输属性列表，使用并行I/O
    hid_t memspace_id = H5Screate_simple(1, dims + 1, NULL);
    hid_t xfer_plist_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(xfer_plist_id, H5FD_MPIO_COLLECTIVE);

    // 选择超立方体
    hsize_t count[2] = {1, 10};
    hsize_t offset[2] = {static_cast<hsize_t>(mpi_rank), 0};
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, NULL, count, NULL);

    // 写入数据
    H5Dwrite(dataset_id, H5T_NATIVE_INT, memspace_id, dataspace_id, xfer_plist_id, data);

    // 关闭资源
    H5Pclose(xfer_plist_id);
    H5Sclose(memspace_id);
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_id);

    // 结束MPI
    MPI_Finalize();

    std::cout << "Process " << mpi_rank << " has written its data." << std::endl;

    return 0;
}

