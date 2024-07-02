/**
 * @Author:   Gao Fei
 * @Date:     2022-02-22 11:30:38
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: BalanceSchedule.h
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-03-03 14:48:18
 *  
 * @Description: 
 *  
 */

#ifndef BalanceSchedule_H
#define BalanceSchedule_H

#include "sharedLBTypes.h"
#include "Partitioner.h"

enum class ResourceType
{
    exclusive,
    shared
};

typedef label ResourceAmt;
typedef label ResourceTag;

struct ResourceGroup
{
    /**
     * @brief      amount of source in this group, for example the number of
     *             processor
     */
    ResourceAmt procsNum;

    /**
     * @brief      tag of every source unit in this group, for example the id of
     *             processor
     *
     * @todo       compress
     */
    std::vector<ResourceTag> procsId;

    /**
     * @brief      some source unit is assign to two or more region
     */
    //std::vector<ResourceTag> nonExcluId; 
    ResourceType procsType;
};

typedef Table<Word, ResourceGroup> ResourceMap;

class BalanceSchedule
{
    // - core data
    Partitioner& pr;

    /**
     * @brief      core assignment: source group (core number .eg) for every
     *             region
     *
     * @todo       dynamic core assignment within a cycle typedef Table<Word,
     *             ResourceGroup> ResourceAssign; std::vector<ResourceAssign>
     *             procAssign;
     */
    ResourceMap procAssign;

    /**
     * @brief      region assignment: regions for every core
     *
     * @todo       compress
     */
    std::vector<std::vector<Word> > regionAssign;

public:
    // - constructor
    BalanceSchedule(Partitioner& pr_);

    // - destructor
    ~BalanceSchedule(){};

    // - member functions
    void process();

    // - member functions access
    ResourceMap& getProcAssign()
    {
        return procAssign;
    }

    void showAssign();
};


#endif

