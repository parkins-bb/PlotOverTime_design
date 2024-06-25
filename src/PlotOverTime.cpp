#include "PlotOverTime.hpp"

PlotOverTime::PlotOverTime(const std::string &dump_directory_name, HSF::Region* reg)
    : dumpDirectoryName(dump_directory_name), region(reg) {}

PlotOverTime::~PlotOverTime() {
    for (auto& file : outputFiles) {
        if (file.second.is_open()) {
            file.second.close();
        }
    }
}

void PlotOverTime::registerByStep(int id, const HSF::SetType type, int time_step) {
    // 将SetType转换为字符串，便于输出文件命名
    std::string typeStr = toString(type);
    // 构造输出文件名
    std::string filename = dumpDirectoryName + "/set_" + std::to_string(id) + "_" + typeStr + ".csv";

    // 检查是否已经存在该id的输出文件
    if (outputFiles.find(id) == outputFiles.end()) {
        // 如果不存在，则创建新的输出文件并写入标题
        outputFiles[id].open(filename);
        outputFiles[id] << "TimeStep, ID, Type" << std::endl;
    }
    // 将id, type和time_step存储在registerSteps容器中
    registeredSteps[id] = std::make_pair(type, time_step);
    // 写入当前时间步的信息
    outputFiles[id] << time_step << ", " << id << ", " << typeStr << std::endl;
}

void PlotOverTime::registerByTime(int id, const HSF::SetType type, double time) {
    // 将SetType转换为字符串，便于输出文件命名
    std::string typeStr = toString(type);
    // 构造输出文件名
    std::string filename = dumpDirectoryName + "/set_" + std::to_string(id) + "_" + typeStr + ".csv";
    
    // 检查是否已经存在该id的输出文件
    if (outputFiles.find(id) == outputFiles.end()) {
        // 如果不存在，则创建新的输出文件并写入标题
        outputFiles[id].open(filename);
        outputFiles[id] << "Time, ID, Type" << std::endl;
    }
    
    // 记录注册信息，便于后续使用
    registeredSteps[id] = std::make_pair(type, static_cast<int>(time));
    // 写入当前时间的信息
    outputFiles[id] << time << ", " << id << ", " << typeStr << std::endl;
}


/*
 * @brief 注册场变量（默认Region中所有相关场），要求每个注册过的id都要调用该函数
 * @param id: 点/线/面/体的index; SetType: 类型
 * @return 将id, SetType, 相关场数据存入csv
 * */
void PlotOverTime::registerPlotFields(const int id, const HSF::SetType type) {
    std::pair<int, HSF::SetType> key = std::make_pair(id, type);
    // 获取指定id和SetType的场索引
    std::vector<int> fieldIndices;
    // 获取指定id和SetType的场名
    std::vector<std::string> fieldNames;
    // 获取指定id和SetType的场数据
    std::vector<std::vector<double>> fieldsData;

    for (int i = 0; i < region->getFieldNum(); i++){
        auto& field = region->getField(i);  // 直接获取场对象
        if (field.getSetType() == type) {
            fieldIndices.push_back(i);
            fieldNames.push_back(field.getName());

            // 获取场数据并存储在data中
            std::vector<double> data(field.size());
            data.reserve(field.size());
            for (int j = 0; j < field.size(); ++j) {
                if (field.getIndex(j) == id) {  // 假设有 getIndex 方法检查 ID
                    data.push_back(field.getValue(j));  // 假设有 getValue 方法获取数据
                }
            }
            fieldsData.push_back(data);
        }
    }

    // 检查是否已注册该场变量
    if (registeredFields_.find(key) == registeredFields_.end()) {
        registeredFields_[key] = fieldIndices;
        registeredFieldNames_[key] = fieldNames;
        fieldData_[key] = fieldsData;
        std::cout << "Registered fields for ID " << id << " and SetType " << type << std::endl;
    } else {
        std::cout << "Fields for ID " << id << " and SetType " << type << " are already registered." << std::endl;
    }
}


std::string PlotOverTime::toString(HSF::SetType type) {
    switch (type) {
        case HSF::SetType::Node:
            return "Node";
        case HSF::SetType::Edge:
            return "Edge";
        case HSF::SetType::Face:
            return "Face";
        case HSF::SetType::Cell:
            return "Cell";
        default:
            return "Unknown element";
    }
}

