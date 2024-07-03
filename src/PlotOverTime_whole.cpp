#include "PlotOverTime.hpp"

namespace HSF {

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
 * @note 暂时不用
 * */
/*
void PlotOverTime::registerByTime(int id, const HSF::SetType type, int time_step) {
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
*/

/*
 * @brief 注册场变量（默认Region中所有相关场），要求每个注册过的id都要调用该函数
 * @param id: 点/线/面/体的index; SetType: 类型
 * @return 将id, SetType对应的所有timestep相关场数据存入csv
 * */
void PlotOverTime::registerPlotFields(const int id, const HSF::SetType type) {
    std::pair<int, HSF::SetType> key = std::make_pair(id, type);

    std::vector<int> validFieldIds;  // 存储有效的场变量ID
    std::vector<std::string> fieldNames;  // 存储场变量名称

    for (const auto& fieldEntry : region->fields_) {
        int fieldId = fieldEntry.first;
        if (region->fieldsType_[fieldId] == typeid(scalar) || region->fieldsType_[fieldId] == typeid(label)) {
            validFieldIds.push_back(fieldId);
            void* fieldPtr = region->fields_[fieldId];
            std::type_index fieldType = region->fieldsType_[fieldId];

            CASTOP(fieldPtr, fieldType, {
                fieldNames.push_back(ft->getName());
            });
        }
    }

    registeredFields_[key] = validFieldIds;
    registeredFieldNames_[key] = fieldNames;
    std::cout << "Registered all fields for ID " << id << " and SetType " << type << std::endl;
}

/*
 * @brief 注册场变量，指定id, SetType, fields_id的场变量，进行输出
 * @param id:点/线/面/体的编号; SetType:点/线/面/体的类型; fields_ids:要注册场变量的id列表
 * @return 将指定id, SetType对应的所有time_Step对应的指定field_ids场数据存储在CSV中
 * */
void PlotOverTime::registerPlotFields(const int id, const SetType type, std::vector<int> &field_ids) {
    std::pair<int, HSF::SetType> key = std::make_pair(id, type);

    std::vector<int> validFieldIds;  // 存储有效的场变量ID
    std::vector<std::string> fieldNames;  // 存储场变量名称

    for (int fieldId : field_ids) {
        if (region->fieldsType_.find(fieldId) != region->fieldsType_.end() && 
            (region->fieldsType_[fieldId] == typeid(scalar) || region->fieldsType_[fieldId] == typeid(label))) {
            validFieldIds.push_back(fieldId);
            void* fieldPtr = region->fields_[fieldId];
            std::type_index fieldType = region->fieldsType_[fieldId];

            CASTOP(fieldPtr, fieldType, {
                fieldNames.push_back(ft->getName());
            });
        }
    }

    registeredFields_[key] = validFieldIds;
    registeredFieldNames_[key] = fieldNames;
    std::cout << "Registered specified fields for ID " << id << " and SetType " << type << std::endl;
}


/*
 * @brief 将特定时间步time_step上对应的场变量数据写入缓存
 * @note 该函数不会写入所有的点/线/面/体上的数据，而是只写入注册过的id和SetType的特定场变量的数据
 * @note 这些数据会被存储在fieldData_容器中，指导调用flushData函数将它们输出到文件中
 * @param time_step:特定的时间步
 * @return 该函数具体的返回下面将提供一个例子以供参考
 * */
void PlotOverTime::writeData(int time_step) {
    for (int ts = 0; ts <= time_step; ++ts) {
        for (const auto& entry : registeredSteps) {
            if (entry.first > time_step) {
                continue;
            }

            int id = entry.second.first;
            HSF::SetType type = entry.second.second;

            auto key = std::make_pair(id, type);
            auto fieldsIt = registeredFields_.find(key);
            if (fieldsIt != registeredFields_.end()) {
                std::vector<int> field_ids = fieldsIt->second;

                std::vector<double> rowData;
                rowData.push_back(static_cast<double>(ts));

                for (int field_id : field_ids) {
                    if (region->fields_.find(field_id) != region->fields_.end()) {
                        void* fieldPtr = region->fields_[field_id];
                        std::type_index fieldType = region->fieldsType_[field_id];

                        CASTOP(fieldPtr, fieldType, {
                            for (size_t j = 0; j < ft->size(); ++j) {
                                if (j == id) {
                                    rowData.push_back(static_cast<double>(field_id));  // FieldID
                                    rowData.push_back(static_cast<double>(ft->operator[](j)));  // Value
                                }
                            }
                        });
                    }
                }

                // 将数据存储在缓存中
                fieldData_[key].push_back(rowData);
            }
        }
    }
}


/*
 * @brief 将缓存中的数据写入相应的CSV文件
 * */
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

        // 写入表头
        file << "TimeStep";
        for (size_t i = 0; i < registeredFields_[key].size(); ++i) {
            int fieldId = registeredFields_[key][i];
            file << ", FieldID(" << fieldId << "), Value(" << fieldId << ")";
        }
        file << "\n";

        // 写入数据
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
            return "NODE";
        case HSF::SetType::Edge:
            return "EDGE";
        case HSF::SetType::Face:
            return "FACE";
        case HSF::SetType::Cell:
            return "CELL";
        default:
            return "Unknown element";
    }
}

} // namespace HSF
