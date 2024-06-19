 曲线数据（时间历程）输出功能

# 需求分析 

## 功能分析

 - 功能需求简单整理如下：  

 1. 输出指定set_id集合（可以是点/线/面/体，必须是存在的Subset）上的所有场信息；  
 2. 输出指定set_id集合（可以是点/线/面/体）上的给定场信息；  
 3. 输出指定若干id（点/线/面/体）上的所有场信息（统一按局部编号）；  
 4. 输出指定若干id（点/线/面/体）上的给定场信息（统一按局部编号）；  
 5. 输出除场信息外，需要默认包含一些元数据信息，如坐标、时间步、当前模拟时间;  
 6. 输出参考量的时间历程（常量、函数等）;  
 7. 输出控制（按时间步还是模拟时间等）；  
 8. 根据指定几何坐标/boundingBox进行输出；（暂时不用考虑，基于前面的功能+空间搜索即可实现）  
 

 - 注意点：  

 1. 前2点根据subset输出，数据量较大，需要用二进制（hdf5）；  
 2. 二进制要能被paraview读取，且方便用户选择点进行绘图；  
 3. 从用户层考虑，最方便的是指定坐标范围输出，为了实现该功能首先需要满足需求3、4；  
 4. 时刻注意网格等数据都是分布式并行的；  
 5. 指定点输出考虑到量较小、且用户使用该接口意味着不想再在paraview中选点，故最好各点独自输出到单独文件；  
 6. set_id集合输出方式可能会有部分数据重复（点/线/面时，不包含ghost但仍有shared），可以暂时不用管（要处理的话可用sFaceId等变量筛选、只有owner能输出shared元素）；  

## 性能需求

- 各进程输出完全独立，只要代码实现不犯低级错误，性能不会差；  
 
# 设计说明

## 文件格式说明

- h5part

采用h5part格式作为set_id集合输出的文件，主要优点： 1. HDF5，高效；2. 单文件； 3. paraview可视化支持。  目前生成h5part文件的代码有两种，参考test的`testH5Part`和`testH5Part2`案例，**这里第一种采用的h5part工具的输出接口（代码已集成至hsf）；第二种是用hsf自己实现的hdf5输出写的，展示了如何将AOS转换为SOA**，因为h5part要求必须为SOA，矢量场必须以各分量的形式输出。
该格式的结构如下：  

```
├── Step#0
│   ├── id
│   ├── x
│   ├── y
│   ├── z
│   └── ...  
│   
├── Step#10
│   
├── ...
```

每个时间步有一个HDF Group，内部包含`id`、坐标与其他场变量，这些量均为1维数组，长度为各进程set_id元素个数的总和。（问题：模拟时间t能否加入该h5part文件的每个step，目前h5part代码似乎有WriteFieldAttrib的功能可用来做这个）  


- csv

该格式为个别点的输出服务，考虑到数据量不大，为了方便处理与可视化采用csv格式，示例如下：  

```
"Point Coordinates:0","Point Coordinates:1","Point Coordinates:2","Time","acc_contact_force:0","acc_contact_force:1","acc_contact_force:2","applied_force:0","applied_force:1","applied_force:2","contatc_force:0","contatc_force:1","contatc_force:2","internal_force:0","internal_force:1","internal_force:2"
-5.0288,-0.09704,-8.0209,0,0,0,0,0,0,0,0,0,0,0,0,0,
-4.9274,-0.09704,-8.0209,0.0020275,0,0,0,0,0,0,0,0,0,-2.1736e-06,1.643e-07,-1.0302e-07,
...
```

命名可灵活调整，易懂就行，注意时间步和模拟时间数据都要有（示例可能不全）  


## 接口设计


### 用户接口

（重要）

```cpp
class PlotOverTime { ///该类只有一个实例
public:

  ///输出目录名,Region中可获得网格及场数据
  PlotOverTime(const string &dump_directory_name,Region* reg);
  
  virtual ~PlotOverTime();


  ///set_id:subset id, type:subset 类型，time_step:输出间隔，time:输出间隔
  ///用于h5part输出（多个不同set_id需输出时，多次调用该函数）
  void registerSetByStep(const int set_id, const SetType type,int time_step);
  void registerSetByTime(const int set_id, const SetType type,scalar time);

  ///id: 输出点编号，type:输出点类型，
  ///用于csv输出（每个点一个文件）（该函数可多次调用，不同采样点设置不同输出间隔）
  void registerByStep(int id, const SetType type,int time_step);
  void registerByTime(int id, const SetType type,scalar time);

  /// 注册场变量（默认Region中所有相关场），要求每个注册过的id都要调用该函数
  void registerSetPlotFields(const int set_id, const SetType type);
  void registerPlotFields(const int id, const SetType type);
  ///相对于上面的接口，增加了field_ids用于指定输出的场变量（注意场的id对应的场类型与type要对应）
  void registerSetPlotFields(const int set_id, const SetType type,std::vector<int> &field_ids);
  void registerPlotFields(const int id, const SetType type,std::vector<int> &field_ids);

  ///为已经调用过registerByStep/Time的id注册一个已知函数f(x,t)场用于和其他场变量对比，x为点的坐标,t为模拟时间，模板T可为label/scalar/Tensor1<scalar>/Tensor1<label>类型
  template<typename T>
  void registerSetVariable(const int set_id, const SetType type,const string &VariableName, const Coefficient<T>& f);
  template<typename T>
  void registerVariable(const int id, const SetType type,const string &VariableName, const Coefficient<T>& f);

  /*!
   * @brief 输出曲线量到缓存
   * @param time_step        输出时的时间步
   * @param simulation_time  输出时的物理时刻
   * */
  void writeData(int time_step, scalar simulation_time);

  /*!
  * @brief 将缓存的曲线量输出到数据文件.
  */
  void flushData();

```


### 内部接口

- h5part相关使用接口（参考testH5Part.cpp案例与h5part文件夹的代码）

```C
H5PartFile *H5PartOpenFileParallel(
    const char *filename, /*!< [in] The name of the data file to open. */
    const char flags,     /*!< [in] The access mode for the file. */
    MPI_Comm comm         /*!< [in] MPI communicator */
);
h5part_int64_t H5PartSetStep(
    H5PartFile *f,            /*!< [in]  Handle to open file */
    const h5part_int64_t step /*!< [in]  Time-step to set. */
) ;
h5part_int64_t H5PartSetNumParticles(
    H5PartFile *f,                  /*!< [in] Handle to open file */
    const h5part_int64_t nparticles /*!< [in] Number of particles */
) ;
h5part_int64_t H5PartWriteDataFloat64(
    H5PartFile *f,                /*!< [in] Handle to open file */
    const char *name,             /*!< [in] Name to associate array with */
    const h5part_float64_t *array /*!< [in] Array to commit to disk */
) ;
h5part_int64_t H5PartWriteDataInt32(
    H5PartFile *f,              /*!< [in] Handle to open file */
    const char *name,           /*!< [in] Name to associate array with */
    const h5part_int32_t *array /*!< [in] Array to commit to disk */
) ;
h5part_int64_t H5PartCloseFile(
    H5PartFile *f /*!< [in] filehandle of the file to close */
) ;

...
```

- HDFTool相关接口

- ustrMesh获取数据相关接口

- Region中获取场的相关接口

（注意Region不能直接知道每个场的类型，因为是通过void*指针存的，需要用fieldsType_的type_index进行判断，详见hsfRegion.cpp的CASTOP宏）

- subset相关接口

### 相关链接

<https://github.com/zoziha/h5part>  
<https://dav.lbl.gov/archive/Research/AcceleratorSAPP/index.html>  

