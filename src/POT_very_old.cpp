#include <string>
#include <unordered_map>
#include <vector>
#include "hdf5.h"
#include "H5Part.h"
#include "Region.h"
#include "pugixml.hpp"   // 用于解析XML文件
#include "ustrXdmfIO.hpp"  // 使用XdmfTool类来解析XML文件

class PlotOverTime{
    public:
        /*
         * @brief 构造函数主要用于初始化类的成员变量
         * @param1 表示输出目录的名称, std::string &dump_directory_name;
         * @param2 指向Region对象指针，用于在类的成员函数中访问网格和场数据, Region* reg;
         * @param3 xdmf文件路径，xdmf_filepath。
         * */
        PlotOverTime(const std::string &dump_directory_name, Region* reg, const std::string &xdmf_filepath)
            : dump_directory_name_(dump_directory_name), region_(reg), xdmf_filepath_(xdmf_filepath), xdmf_tool_(nullptr) {   
            // 创建一个新的UstrXdmfTool对象，并将其指针赋给xdmf_tool_
            xdmf_tool_ = new HSF::IO::UstrXdmfTool(nullptr, xdmf_filepath_, "r", HSF::IO::XdmfTool::HDF5, false);  // 初始化XdmfTool对象
            // 解析XDMF文件，获取HDF5文件路径
            parseXdmfFile();
            // 初始化H5Part文件
            initH5Part();
        }

        /*
         * @brief 析构函数
         * */
        virtual ~PlotOverTime() {
            closeH5Part();
            delete xdmf_tool_;  // 清理xdmf_tool_对象
        };

        /*
         * @brief 成员函数1，输出指定set_id集合上的所有场信息
         * @param1
         * */
        void outputAllFields(const std::vector<int>& setIds, const SetType type);

    private:
        std::string dump_directory_name_;  // 输出目录名称
        Region* region_;                   // 指向Region对象指针
        std::string xdmf_filepath_;        // xmf文件的路径
        std::string h5_filename_;          // h5文件的名称
        H5PartFile* h5file_;               // 指向h5文件的路径
        HSF::IO::UstrXdmfTool* xdmf_tool_;              // 
        
        std::unordered_map<std::pair<int, SetType>, int> setByStep_;

        // 之后可能会涉及的成员变量
        std::unordered_map<std::pair<int, SetType>, scalar> setByTime_;
        std::unordered_map<std::pair<int, SetType>, int> byStep_;
        std::unordered_map<std::pair<int, SetType>, scalar> byTime_;
        std::unordered_map<std::pair<int, SetType>, std::vector<int>> plotFields_;
        std::unordered_map<std::pair<int, SetType>, std::unordered_map<std::string, Coefficient<>>> setVariables_;
        std::unordered_map<std::pair<int, SetType>, std::unordered_map<std::string, Coefficient<>>> variables_;

        // 辅助函数:获取并输出场数据
        void writeFieldData(const Subset& subset);
        // 初始化H5Part文件，打开h5文件以供读取
        void initH5Part();
        void closeH5Part();
        // 解析xdmf文件，获取h5文件的名称
        void parseXdmfFile();
}

/*
 * @brief 解析xdmf文件，获取h5文件的路径;
 * @input xdmf_filepath_: 'std::string'类型，'xdmf'文件的路径，这个路径在构造函数中通过初始化列表传给成员变量
 * @output h5_filename_: 'std::string'类型，'h5'文件的名称，这个值将在本函数中被解析并存储到成员变量中
 */
void PlotOverTime::parseXdmfFile() {
    // 使用pugixml解析XDMF文件
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(xdmf_filepath_.c_str());

    if (!result) {
        throw std::runtime_error("Failed to open or parse XDMF file");
    }

    // 查找HDF5文件的名称示例, 其中h5文件在xmf中的节点路径为Xdmf/Domain/Grid/Grid/Geometry/DataItem
    pugi::xml_node xdmf = doc.child("Xdmf");
    pugi::xml_node domain = xdmf.child("Domain");
    pugi::xml_node grid = domain.child("Grid");
    pugi::xml_node geometry = grid.child("Grid").child("Geometry");
    pugi::xml_node dataItem = geometry.child("DataItem");

    // 使用xdmf_tool_对象的getHDF5Paths函数获取HDF5文件路径并赋值给h5_filename_
    std::array<std::string, 2> paths;
    xdmf_tool_->getHDF5Paths(paths, dataItem);
    h5_filename_ = paths[0];
}

/*
 * @brief 初始化H5Part文件
 * */
void PlotOverTime::initH5Part() {
    // 打开H5Part文件，并将文件句柄赋值给h5file_
    h5file_ = H5PartOpenFileParallel(h5_filename_.c_str(), H5PART_READ, MPI_COMM_WORLD);
    if (h5file_ == nullptr) {
        throw std::runtime_error("Failed to open H5Part file");
    }
}

/*
 * @brief 关闭H5Part文件
 * */
void PLotOverTime::closeH5Part() {
    // 关闭H5Part文件
    if (h5file_ != nullptr) {
        H5PartCloseFile(h5file_);
    }
}

/*
 * @brief 输出指定 set_id 集合上所有场信息 
 * @param1 setIds: 包含要输出的set_id的集合
 * @param2 type: set_id的类型（例如点、线、面、体）
 * */
void PlotOverTime::outputAllFields(const std::vector<int>& setIds, const SetType type) {
    // 遍历setIds集合
    for (const int setId : setIds) {
        // 获取指定 set_id 的 Subset
        Subset* subset = region_->getSubset(setId, type);
        if (subset) {
            // 输出该Subset上的所有场数据
            writeFiledData(*subset);
        } else {
            std::cerr << "Subset with ID " << setId << " and type " << type << " not found. " << std::endl;
        }
    }
}

/*
 * @brief 获取并输出Subset上所有场数据
 * @param1 要输出场数据的Subset对象
 * */
void PlotOverTime::writeFieldData(const Subset& subset) {
    // 获取Subset上所有元素
    const std::vector<label>& indices = subset.getArray();
    for (label index : indices) {
        // 获取每个元素上的所有场数据
        for (const Field& field : region_->getFields()) {
            // 检查场数据是否在该Subset上
            if (filed.getSetType() == subset.getSetType()) {
                // 输出场数据
                std::cout << "Field: " << field.getName() << ", Value: " << field.getValue(index) << std::endl;
            }
        }
    }
}
