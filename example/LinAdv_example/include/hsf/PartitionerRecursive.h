/**
 * @Author:   Gao Fei 
 * @Date:     2022-04-01 21:42:22 
 * @E-mail:   gaofei@mail.nsccwx.cn 
 * @Project:  hsf_edit 
 * @FileName: PartitionerRecursive.h
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-04-02 16:35:18
 *
 * @Description: multi-domian recursive partition load balance method is
 * implemented
 */

#ifndef PartitionerRecursive_H
#define PartitionerRecursive_H

#include "sharedLBTypes.h"

class PartitionerRecursive
{
    // - core member data
    std::vector<scalar> levelLoad;
    std::vector<scalar> realResrc;
    std::vector<label>  levelResrc;
    label levelNum;
    label procsNum;

public:
    // - constructors and destructor
    PartitionerRecursive();
    ~PartitionerRecursive(){};

    // - member functions operations
    void calcLevelsLoad();
    void assignResource();

    // - member functions access

private:
    // - member functions relization
    scalar calcRi(int il, scalar& factor);

    // - member data relization
    scalar R;

};

#endif

