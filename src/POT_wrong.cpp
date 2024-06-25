#include "PlotOverTime.hpp"

// 构造函数
PlotOverTime::PlotOverTime(Communicator* comm, const std::string& xmfFileName)
    : xdmfIO_(comm, xmfFileName) {
    // 解析XMF文件,并初始化setIdPaths
    setIdPaths_ = parseXmfFile();
}

PlotOverTime::~PlotOverTime() {}

/*
 * @brief 解析xmf的节点
 * */
std::map<int, std::vector<std::string>> PlotOverTime::parseXmffile() {
    std::map<int, std::vector<std::string>> setIdPaths;

    // 获取XML文档对象
    pugi::xml_document* xml_doc = xdmfIO_.getXmlDocument();
    // 获取XML文档根节点
    pugi::xml_node rootNode = xml_doc->child("Xdmf").child("Domain");

    // 遍历子集
    for (pugi::xml_node grid : rootNode.children("Grid")) {
        if (std::string(grid.attribute("Name").value()) == "subset") {
            for (pugi::xml_node subset : grid.children("Grid")) {
                int setId = std::stoi(subset.attribute("ID").value());
                std::vector<std::string> paths;

                // 获取Geometry 和Topology路径
                std::string geometryPath = subset.child("Geometry").child("DataItem").text().get();
                std::string topologyPath = subset.child("Topology").child("DataItem").text().get();

                // 将路径添加到向量中
                paths.push_back(geometryPath);
                paths.push_back(topologyPath);

                // 将路径存储到setIdPaths中
                setIdPaths[setId] = paths;
            }
        }
    }
    return setIdPaths;
}

/*
 * @brief 获取
 * */
std::map<std::string, std::string> PlotOverTime::getDatasetPaths(int setId) {
    std::map<std::string, std::string> datasetPaths;

    if (setIdPaths_.find(setId) != setIdPaths_.end()) {
        auto paths = setIdPaths_[setId];
        
        // 遍历几何和拓扑路径，并存储到datasetPaths中
        for (const auto& path : paths) {
            datasetPaths["Geometry"] = path;
            datasetPaths["Topology"] = path;
        }

        // 获取attributeID, processID, cell_velocity等信息
        pugi::xml_document* xml_doc = xdmfIO_.getXmlDocument();
        pugi::xml_node rootNode = xml_doc->child("Xdmf").child("Domain");

        for (pugi::xml_node grid : rootNode.children("Grid")) {
            if (std::string(grid.attribute("Name").value()) == "internalMesh") {
                for (pugi::xml_node attribute : grid.children("Attribute")) {
                    std::string name = attribute.attribute("Name").value();
                    std::string path = attribute.child("DataItem").text().get();
                    datasetPaths[name] = path;
                }
            }
        }
    }

    return datasetPaths;
}

void PlotOverTime::loadDataFromH5(int setId) {
    auto datasetPaths = getDatasetPaths(setId);
    std::map<std::string, std::vector<double>> data;

    for (const auto& entry : datasetPaths) {
        const std::string& dataName = entry.first;
        const std::string& hdf5Path = entry.second;
        std::array<std::string, 2> paths;
        xdmfIO_.getHDF5Paths(paths, hdf5Path);
        std::string h5FileName = paths[0];
        std::string h5DataPath = paths[1];

        HDF5Tool hdf5Tool(h5FileName);
        std::vector<double> dataset;
        hdf5Tool.readDataset(h5DataPath, dataset);
        data[dataName] = dataset;
    }

    setIdData_[setId] = data;
}

void PlotOverTime::writeDataToHDF5(int setId, const std::string& outputFileName) {
    if (setIdData_.find(setId) == setIdData_.end()) {
        std::cerr << "Data for set_id " << setId << " not loaded." << std::endl;
        return;
    }

    HDF5Tool hdf5Tool(outputFileName, "w");

    for (const auto& entry : setIdData_[setId]) {
        const std::string& dataName = entry.first;
        const std::vector<double>& data = entry.second;
        hdf5Tool.writeDataset(dataName, data);
    }
}

void PlotOverTime::writeSpecifiedFieldToHDF5(int setId, const std::string& fieldName, const std::string& outputFileName) {
    if (setIdData_.find(setId) == setIdData_.end()) {
        std::cerr << "Data for set_id " << setId << " not loaded." << std::endl;
        return;
    }

    if (setIdData_[setId].find(fieldName) == setIdData_[setId].end()) {
        std::cerr << "Field " << fieldName << " not found for set_id " << setId << "." << std::endl;
        return;
    }

    HDF5Tool hdf5Tool(outputFileName, "w");

    const std::vector<double>& data = setIdData_[setId][fieldName];
    hdf5Tool.writeDataset(fieldName, data);
}


