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

/*
 * @brief 将特定的id, SetType, time_step存储在容器中，并输出到CSV文件
 * */
void PlotOverTime::registerByStep(int id, const HSF::SetType type, int time_step) {
    // 将SetType转换为字符串，便于输出文件命名
    std::string typeStr = toString(type);
    // 构造输出文件名
    std::string filename = dumpDirectoryName + "/set_" + std::to_string(id) + "_" + typeStr + ".csv";

    // 检查是否已经存在该id的输出文件
    if (outputFiles.find(id) == outputFiles.end()) {
        // 如果不存在，则创建新的输出文件并写入标题
        outputFiles[id].open(filename);
        // 写入文件标题行
        outputFiles[id] << "TimeStep, ID, Type" << std::endl;
    }
    // 将id, type和time_step存储在registerSteps容器中
    registeredSteps[id] = std::make_pair(type, time_step);
    // 写入当前时间步的信息，将时间步time_step，节点id，和节点类型写入CSV文件
    outputFiles[id] << time_step << ", " << id << ", " << typeStr << std::endl;
}

/*
 * @brief 将特定的id, SetType, time存储在容器中，并输出到CSV文件
 * */
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
    // 生成一个包含id和type的键值对
    std::pair<int, HSF::SetType> key = std::make_pair(id, type);
    // 存取指定id和SetType的场索引
    std::vector<int> fieldIndices;
    // 存取指定id和SetType的场名
    std::vector<std::string> fieldNames;
    // 存取指定id和SetType的场数据
    std::vector<std::vector<double>> fieldsData;
    
    // 遍历Region中的所有场
    for (int i = 0; i < region->getFieldNum(); i++){
        auto& field = region->getField(i);  // 直接获取场对象
        // 如果场的类型和输入类型匹配
        if (field.getSetType() == type) {
            // 将场的索引和名称存储到对应的容器中
            fieldIndices.push_back(i);
            fieldNames.push_back(field.getName());

            // 获取场数据并存储在data中
            std::vector<double> data(field.size());
            data.reserve(field.size());
            for (int j = 0; j < field.size(); j++) {
                if (j == id) {
                    data.push_back(static_cast<double>(field[j]));  // 直接访问数据
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

/*
 * @brief 注册场变量，指定id, SetType, fields_id的场变量，进行输出
 * @param id:点/线/面/体的编号; SetType:点/线/面/体的类型; fields_ids:要注册场变量的id列表
 * @return 将指定id, SetType，Fields存储在CSV中
 * */
void PlotOverTime::registerPlotFields(const int id, const SetType type, std::vector<int> &field_ids) {
    // 创建键，表示特定的id和SetType组合
    std::pair<int, HSF::SetType> key = std::make_pair(id, type);
    
    std::vector<int> validFieldIds;   // 存储有效的场变量ID
    std::vector<std::string> fieldNames;  // 存储场变量名称
    std::vector<std::vector<double>> fieldsData;  // 存储场变量数据

    // 遍历field_ids
    for (int fieldId : field_ids) {
        // 检查场变量ID是否存在并且类型匹配
        if (region->getFieldIndex(fieldId, type) != -1) {
            validFieldIds.push_back(fieldId);
            
            // 获取场变量对象
            auto& field = region->getField(fieldId);
            fieldNames.push_back(field.getName());

            // 获取场数据并存储在data中
            std::vector<double> data(field.size());
            data.reserve(field.size());
            for (int j = 0; j < field.size(); ++j) {
                if (field.getIndex(j) == id) {
                    data.push_back(field.getValue(j));
                }
            }
            fieldsData.push_back(data);  // 将指定id【匹配的数据存储在fieldsData容器中
        }
    }

    if (!validFieldIds.empty()) {
        registeredFields_[key] = validFieldIds;
        registeredFieldNames_[key] = fieldNames;
        fieldData_[key] = fieldsData;
        std::cout << "Registered fields for ID " << id << " and SetType " << type << std::endl;
    } else {
        std::cout << "No valid fields found for ID " << id << " and SetType " << type << std::endl;
    }
}

/*
 * @brief 此函数为了获取节点的坐标值，将坐标值写入CSV文件，并存储在
 * @note 节点的坐标值是实时更新的
 * @param id:节点的坐标; time_step:时间步
 * */
void PlotOverTime::outputNodeCoordinates(int id, int time_step) {
    auto mesh = dynamic_cast<USTRMesh*>(region->getMesh());
    // 确保region中的Mesh对象确实是USTRMesh类型的实例
    if (!mesh) {
        std::cerr << "Region does not contain a USTRMesh." << std::endl;
        return;
    }

    // 创建CSV文件，写入文件的头部
    std::string filename = dumpDirectoryName + "/node_" + std::to_string(id) + "_step_" + std::to_string(time_step) + ".csv";
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file << "TimeStep, NodeID, X, Y, Z" << std::endl;

    // 获取节点坐标
    auto& nodeCoords = mesh->getNodeCoordinates();
    if (nodeCoords.find(id) != nodeCoords.end()) {
        // 将坐标数据写入文件中
        auto& coord = nodeCoords[id];
        file << time_step << ", " << id << ", " << coord[0] << ", " << coord[1] << ", " << coord[2] << std::endl;
        // 将坐标数据存储到fieldData_容器中
        std::pair<int, HSF::SetType> key = std::make_pair(id, HSF::SetType::NODE);
        fieldData_[key].push_back({static_cast<double>(time_strp), coord[0], coord[1],coord[2]});
    } else {
        std::cerr << "Node ID " << id << " not found." << std::endl;
    }

    file.close();
}

/*
 * @brief 将特定时间步time_step上对应的场变量数据写入缓存
 * @note 该函数不会写入所有的点/线/面/体上的数据，而是只写入注册过的id和SetType的特定场变量的数据
 * @note 这些数据会被存储在fieldData_容器中，指导调用flushData函数将它们输出到文件中
 * @param time_step:特定的时间步
 * @return 该函数具体的返回下面将提供一个例子以供参考
 * */
void PlotOverTime::writeData(int time_step) {
    // 遍历registeredSteps, 检查是否有匹配的time_step条目
    for (const auto& entry : registeredSteps) {
        if (entry.first != time_step) {
            continue;
        }

        // 如果找到匹配的时间步，提取对应的id和SetType
        int id = entry.second.first;
        SetType type = entry.second.second;

        // (id, type)作为键
        auto key = std::make_pair(id, type);
        auto fieldsIt = registeredFields_.find(key);
        if (fieldsIt != registeredFields_.end()) {
            std::vector<int> field_ids = fieldsIt->second;

            for (int field_id : field_ids) {
                try {
                    // 获取字段类型并根据类型获取字段数据
                    auto typeIt = region->fieldsType_.find(field_id);
                    if (typeIt == region->fieldsType_.end()) {
                        throw std::runtime_error("Field type not found.");
                    }

                    const std::type_index& fieldType = typeIt->second;
                    std::vector<double> data;

                    if (fieldType == typeid(scalar)) {
                        auto& field = region->getField<scalar>(field_id);
                        for (const auto& value : field) {
                            data.push_back(static_cast<double>(value));
                        }
                    } else if (fieldType == typeid(label)) {
                        auto& field = region->getField<label>(field_id);
                        for (const auto& value : field) {
                            data.push_back(static_cast<double>(value));
                        }
                    } else {
                        throw std::runtime_error("Unsupported field type.");
                    }

                    // 将数据存储在容器中
                    fieldData_[key].push_back(data);

                } catch (const std::exception& e) {
                    std::cerr << "Error processing field id: " << field_id << ": " << e.what() << "\n";
                }
            }
        }

        // 处理节点坐标的特殊情况
        if (type == HSF::SetType::NODE) {
            auto mesh = dynamic_cast<USTRMesh*>(region->getMesh());
            if (mesh) {
                auto& nodeCoords = mesh->getNodeCoordinates();
                if (nodeCoords.find(id) != nodeCoords.end()) {
                    auto& coord = nodeCoords[id];
                    std::vector<double> coordData = { static_cast<double>(time_step), coord[0], coord[1], coord[2] };
                    fieldData_[key].push_back(coordData);
                } else {
                    std::cerr << "Node ID " << id << " not found." << std::endl;
                }
            } else {
                std::cerr << "Region does not contain a USTRMesh." << std::endl;
            }
        }
    }
}


void PlotOverTime::flushData() {
    for (auto& entry : fieldData_) {
        std::pair<int, HSF::SetType> key = entry.first;
        int id = key.first;
        HSF::SetType type = key.second;

        std::string typeStr = toString(type);
        std::string filename = dumpDirectoryName + "/set_" + std::to_string(id) + "_" + typeStr + ".csv";
        std::ofstream file(filename, std::ios::app);

        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            continue;
        }

        for (const auto& row : entry.second) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) {
                    file << ", ";
                }
            }
            file << "\n";
        }

        file.close();
    }

    // 清空缓存
    fieldData_.clear();
}

/*******************************************使用示例********************************************
假设使用了SetType::NODE类型记录下了field_ids为{0, 1, 2}的场变量数据，
并且使用了SetType::FACE类型记录下了field_ids为{3, 4}的场变量数据
 * 在time_step为10的SetType::NODE类型下:
   * Field 0: [1.0, 2.0, 3.0]
   * Field 1: [4.0, 5.0, 6.0]
   * Field 2: [7.0, 8.0, 9.0]
 * 在time_step为20的SetType::FACE类型下:
   * Field 3: [0.1, 0.2, 0.3]
   * Field 4: [0.4, 0.5, 0.6]
那么生成的set_1_Node.csv文件为:
    TimeStep: 10, ID: 1, Type: Node
    1.0, 2.0, 3.0,
    4.0, 5.0, 6.0,
    7.0, 8.0, 9.0,
生成的set_2_Face.csv文件为:
    TimeStep: 20, ID: 2, Type: Face
    0.1, 0.2, 0.3,
    0.4, 0.5, 0.6,
***********************************************************************************************/


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

