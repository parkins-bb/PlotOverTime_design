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

    for (int i = 0; i < region->getFieldNum(); ++i) {
        try {
            auto& field = region->getField<scalar>(i);
            if (field.getSetType() == type) {
                validFieldIds.push_back(i);
                fieldNames.push_back(field.getName());
            }
        } catch (...) {
            try {
                auto& field = region->getField<label>(i);
                if (field.getSetType() == type) {
                    validFieldIds.push_back(i);
                    fieldNames.push_back(field.getName());
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

    for (int fieldId : field_ids) {
        try {
            auto& field = region->getField<scalar>(fieldId);
            if (field.getSetType() == type) {
                validFieldIds.push_back(fieldId);
                fieldNames.push_back(field.getName());
            }
        } catch (...) {
            try {
                auto& field = region->getField<label>(fieldId);
                if (field.getSetType() == type) {
                    validFieldIds.push_back(fieldId);
                    fieldNames.push_back(field.getName());
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
                        for (size_t j = 0; j < field.size(); ++j) {
                            if (static_cast<int>(j) == id) {
                                rowData.push_back(static_cast<double>(field_id));  // FieldID
                                rowData.push_back(static_cast<double>(field[j]));  // Value
                            }
                        }
                    } catch (...) {
                        try {
                            auto& field = region->getField<label>(field_id);
                            for (size_t j = 0; j < field.size(); ++j) {
                                if (static_cast<int>(j) == id) {
                                    rowData.push_back(static_cast<double>(field_id));  // FieldID
                                    rowData.push_back(static_cast<double>(field[j]));  // Value
                                }
                            }
                        } catch (...) {
                            // Handle other types if needed
                        }
                    }
                }

                fieldData_[key].push_back(rowData);
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

