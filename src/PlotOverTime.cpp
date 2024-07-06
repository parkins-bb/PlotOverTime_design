#include "PlotOverTime.hpp"

namespace HSF {

// 构造
PlotOverTime::PlotOverTime(Region* reg, const std::string& directory)
    : region(reg), dumpDirectoryName(directory) {
}

// 析构
PlotOverTime::~PlotOverTime() {
}

void PlotOverTime::registerByStep(int id, const HSF::SetType type, int time_step) {
    registeredSteps[time_step] = std::make_pair(id, type);
}

void PlotOverTime::registerByTime(int id, const HSF::SetType type, int time_step) {
    registeredSteps[time_step] = std::make_pair(id, type);
}

void PlotOverTime::registerPlotFields(int id, const HSF::SetType type) {
    std::pair<int, HSF::SetType> key = std::make_pair(id, type);

    std::vector<int> validFieldIds;
    std::vector<std::string> fieldNames;

    std::cout << "Attempting to register plot fields for ID " << id << " and SetType " << type << std::endl;

    for (int i = 0; i < region->getFieldNum(); ++i) {
        try {
            auto& field = region->getField<scalar>(i);
            if (field.getSetType() == type) {
                validFieldIds.push_back(i);
                fieldNames.push_back(field.getName());
                std::cout << "Registered scalar field: " << field.getName() << " with ID: " << i << std::endl;            
                    
                // 输出字段内容
                std::cout << "Field " << field.getName() << " content: ";
                for (size_t j = 0; j < field.size(); j++) {
                    std::cout << field[j] << " ";
                }
                std::cout << std::endl;
            }
        } catch (...) {
            try {
                auto& field = region->getField<label>(i);
                if (field.getSetType() == type) {
                    validFieldIds.push_back(i);
                    fieldNames.push_back(field.getName());
                    std::cout << "Registered label field: " << field.getName() << " with ID: " << i << std::endl;

                    // 输出字段内容
                    std::cout << "Field " << field.getName() << " content: ";
                    for (size_t j = 0; j < field.size(); j++) {
                        std::cout << field[j] << " ";
                    }
                    std::cout << std::endl;
                }
            } catch (...) {
                // Handle other types if needed
            }
        }
    }

    registeredFields_[key] = validFieldIds;
    registeredFieldNames_[key] = fieldNames;
    std::cout << "Registered all fields for ID " << id << " and SetType " << type << std::endl;
}

void PlotOverTime::registerPlotFields(int id, const HSF::SetType type, std::vector<int>& field_ids) {
    std::pair<int, HSF::SetType> key = std::make_pair(id, type);

    std::vector<int> validFieldIds;
    std::vector<std::string> fieldNames;

    std::cout << "Attempting to register specific plot fields for ID " << id << " and SetType " << type << std::endl;

    for (int fieldId : field_ids) {
        try {
            auto& field = region->getField<scalar>(fieldId);
            if (field.getSetType() == type) {
                validFieldIds.push_back(fieldId);
                fieldNames.push_back(field.getName());
                std::cout << "Registered scalar field: " << field.getName() << " with ID: " << fieldId << std::endl;
            
                // 输出字段内容
                std::cout << "Field " << field.getName() << " content: ";
                for (size_t j = 0; j < field.size(); j++) {
                    std::cout << field[j] << " ";
                }
                std::cout << std::endl;
            }
        } catch (...) {
            try {
                auto& field = region->getField<label>(fieldId);
                if (field.getSetType() == type) {
                    validFieldIds.push_back(fieldId);
                    fieldNames.push_back(field.getName());
                    std::cout << "Registered label field: " << field.getName() << " with ID: " << fieldId << std::endl;
                
                    // 输出字段内容
                    std::cout << "Field " << field.getName() << " content: ";
                    for (size_t j = 0; j < field.size(); j++) {
                        std::cout << field[j] << " ";
                    }
                    std::cout << std::endl;
                }
            } catch (...) {
                // Handle other types if needed
            }
        }
    }

    registeredFields_[key] = validFieldIds;
    registeredFieldNames_[key] = fieldNames;
    std::cout << "Registered specified fields for ID " << id << " and SetType " << type << std::endl;
}

void PlotOverTime::writeData(int time_step) {
    // 调试输出
    std::cout << "writeData called for time_step: " << time_step << std::endl;
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
                    try {
                        auto& field = region->getField<scalar>(field_id);
                        std::cout << "Field (scalar) ID: " << field_id << ", size: " << field.size() << std::endl;
                        for (size_t j = 0; j < field.size(); ++j) {
                            if (static_cast<int>(j) == id) {
                                rowData.push_back(static_cast<double>(field_id));  // FieldID
                                rowData.push_back(static_cast<double>(field[j]));  // Value
                                std::cout << "Storing value for field " << field_id << ": " << field[j] << std::endl;
                            }
                        }
                    } catch (...) {
                        try {
                            auto& field = region->getField<label>(field_id);
                            std::cout << "Field (label) ID: " << field_id << ", size: " << field.size() << std::endl;
                            for (size_t j = 0; j < field.size(); ++j) {
                                if (static_cast<int>(j) == id) {
                                    rowData.push_back(static_cast<double>(field_id));  // FieldID
                                    rowData.push_back(static_cast<double>(field[j]));  // Value
                                    std::cout << "Storing value for field " << field_id << ": " << field[j] << std::endl;
                                }
                            }
                        } catch (...) {
                            std::cerr << "Failed to get field for ID: " << field_id << std::endl;
                            // Handle other types if needed
                        }
                    }
                }

                std::cout << "Storing data for key: (" << id << ", " << type << ")" << std::endl;
                fieldData_[key].push_back(rowData);
            } else {    
                std::cerr << "No registered fields for key: (" << id << ", SetType: " << type << ")" << std::endl;
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
        
        std::cout << "Writing data to file: " << filename << std::endl;

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
        std::cout << "Data flushed to file: " << filename << std::endl;
    }

    fieldData_.clear();
}

std::string PlotOverTime::toString(HSF::SetType type) {
    switch (type) {
        case HSF::SetType::NODE:
            return "Node";
        case HSF::SetType::EDGE:
            return "Edge";
        case HSF::SetType::FACE:
            return "Face";
        case HSF::SetType::CELL:
            return "Cell";
        default:
            return "Unknown";
    }
}

} // namespace HSF

