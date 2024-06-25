#ifndef SYSTEM_PARAMETER_PARAMETER_HPP
#define SYSTEM_PARAMETER_PARAMETER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "yaml-cpp/yaml.h"

namespace HSF {
class Parameter {
 public:
  // 构造函数:加载一个yaml文件，接受一个yaml文件名作为参数，文件路径默认为../data
  Parameter(const std::string& yamlFilename);

  // 成员函数1： 函数用于获取指定文件夹下指定后缀的所有文件名,
  // 接受文件路径和扩展名作为参数
  std::vector<std::string> getFilesWithExtension(const std::string& folderPath,
                                                  const std::string& extension);

  // 成员函数2：函数用于获取yaml后缀的文件，并输出成字符串数组形式
  std::vector<std::string> searchYamlfiles(const std::string& folderPath);

  // 成员函数3：将数组写入文件的函数
  void writeToFile(const std::vector<std::string>& files,
                   const std::string& outputFileName);

  // 成员函数4：用于获取并返回yaml文件所有的节点内容
  std::vector<std::string> getYamlNodes();

  // 结构体定义，用于返回节点信息，包括节点类型、父节点、子节点
  struct NodeInfo {
    std::string nodeType;
    std::string parentNode;  // 可能没有父节点
    std::vector<std::string> childNodes;
  };

  // 成员函数5: 通过路径找到节点，返回当前节点的类型，父节点、子节点
  // 通过路径搜查的好处是防止搜索到同名节点，保证搜索的准确性，本方法不使用模糊搜索
  NodeInfo getNodeInfoByPath(const std::string& internalPath);

  // 成员函数6：给定目标值，返回该值在YAML结构中的路径
  std::vector<std::string> getNodePathByValue(const std::string& targetValue);

  // 结构体定义，用于存储查找结果
  struct FindResult {
    std::string path;                 // 节点的路径
    std::vector<std::string> values;  // 节点的值，考虑到值可能是一个序列
  };

  // 成员函数7: 此函数为了实现功能：给出一个yaml节点的键名，
  // (1). 输出这个建的路径；
  // (2). 输出这个键所对应的值，如果节点的值对应是序列的话，也全部输出
  // 功能：返回节点值，节点路径，如果节点是序列，则遍历序列元素
  FindResult findAndPrintByKey(const std::string& targetKey);

  // 成员函数8:
  // 本函数为了实现如下功能：给出节点的具体路径，增加这个路径下的节点,
  // 需要调用成员函数9保存至新文件
  // 本函数要求输入yaml文件路径，指定要加入节点的路径，新增节点的名称，以及新增节点的内容
  // 其中AddNodeToYaml函数为实现增加功能的函数
  // 函数，增加节点到YAML中指定的路径
  // pathStr为一个字符串相连，表示节点的路径，每个字符串代表路径中的一个节点名称
  // nodeName是新增加节点的名称，newNode是新增加节点的内容
  bool addNodeToYaml(const std::string& pathStr, const std::string& nodeName,
                     const YAML::Node& newNode);

  // 成员函数9:将当前YAML内容保存到新文件
  // 此函数为了实现多个进程同时修改源文件内容造成写冲突的问题，同时也避免了错误修改造成无法回复源文件的问题
  bool saveYamlToFile(const std::string& newFilename,
                      const std::string& savePath);

  // 成员函数10:
  // 本函数实现了修改节点的功能，需要调用成员函数9保存至新文件
  // 输入文件root、节点路径path、新值节点newValue、新键名newKey（可选）如果需要改映射的键名，请提供新键名
  // 如果节点是标量的话，修改标量的内容
  // 如果节点是序列的话，修改序列的内容
  // 如果节点是映射的话，修改值的内容或修改
  bool modifyNode(const std::string& pathStr, const YAML::Node& newValue,
                  const std::string& newKey = "");

  // 成员函数11: 删除某路径下的节点，需要调用成员函数9保存至新文件
  bool removeNode(const std::string& pathStr);

  // 获取当前操作的YAML文件内容
  YAML::Node getYamlContent() const;

  // 析构函数，输出对象删除信息
  ~Parameter();

 private:
  // loadYamlFile函数, 加载特定路径的yaml文件
  void loadYamlFile(const std::string& yamlFilePath);

  // 函数FindNodeByPath,通过字符串路径找到节点（而非索引路径）
  YAML::Node findNodeByPath(const YAML::Node& root,
                            const std::vector<std::string>& path);

  // 提取节点信息
  NodeInfo extractNodeInfo(const YAML::Node& node);

  // 将节点类型转为字符串
  std::string nodeTypeToString(YAML::NodeType::value type);

  // FindNodePath实现递归查找
  bool findNodePath(const YAML::Node& node, const YAML::Node& target,
                    std::vector<std::string>& path);

  // 辅助递归函数，用于查找键
  bool findByKey(const YAML::Node& node, const std::string& targetKey,
                 const std::string& path, FindResult& result);

  // 实现增加功能
  bool addNode(YAML::Node& root, const std::vector<std::string>& path,
               const std::string& nodeName, const YAML::Node& newNode);

  // 此函数是用于分离路径字符串，比如"Person.age"分离为["Person", "age"]
  std::vector<std::string> splitPath(const std::string& pathStr);

  // 实际执行修改操作的内部函数
  bool modifyNodeInternal(YAML::Node& current,
                          const std::vector<std::string>& path,
                          const YAML::Node& newValue,
                          const std::string& newKey);

  // 成员变量1
  std::vector<std::string> files_;
  // 成员变量2 文件名和YAML内容的映射
  std::unordered_map<std::string, YAML::Node> configs_;
  // 成员变量3 保存当前操作的文件名
  std::string currentYamlFilename_;
};

}  // namespace HSF

#endif /* end of include guard: SYSTEM_PARAMETER_PARAMETER_HPP */
