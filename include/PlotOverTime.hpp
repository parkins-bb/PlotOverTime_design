#include <iostream>
#include "h5part"

class PlotOverTime{
// 该类只有一个实例
public:
    // 输出目录名，Region中可以获得网格及场数据
	PlotOverTime(const string &dump_directory_name, Region* reg);

    virtual ~PlotOverTime();

}

