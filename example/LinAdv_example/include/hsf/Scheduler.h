/**
 * @Author:   Gao Fei
 * @Date:     2022-02-02 10:40:54
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: Scheduler.h
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-03-03 14:52:12
 *  
 * @Description: 
 *  
 */
#ifndef Scheduler_H
#define Scheduler_H

#include "sharedLBTypes.h"
#include "ProcedureGraph.h"
#include "Partitioner.h"
#include "BalanceSchedule.h"

class Scheduler
{
    // - core data
    // input
    ProcedureGraph* pg;

    // output
    BalanceSchedule* bs;

    // self
    Partitioner* pr;  


public:
    // - constructor
    Scheduler()
    {
        pg = new ProcedureGraph();
        pr = new Partitioner(*pg);
        bs = new BalanceSchedule(*pr);
    }

    // - destructor
    ~Scheduler()
    {
        delete pr;
        delete bs;
        delete pg;
    }

    // - member functions
    void process();

    void checkResource();

    ProcedureGraph& getPG()
    {
        return *pg;
    }

    BalanceSchedule& getBS()
    {
        return *bs;
    }
    
};

#endif

/******************************************* old code *********************************/    
    // void constructGlobalWorkFlow();

    // void assignSourceToActivity();

    // void adjustSourceToActivity();

    // void broadcastToAllprocessors();

    // // for unit test
    // void constructSAMRLBMWorkFlow();

    // // - realizations
    // void recursivePartation()
    // {
    //     // get barrier node
    //     BarNodeMap& barMap = getBarNode();

    //     // traverse all the region, 
    //     BarNodeMap::iterator ir1;
    //     for (ir1 = barMap.begin(); ir1 != barMap.end(); ir1++)
    //     {
    //         // traverse the last region
    //         RegNodeMap::iterator ir2;
    //         for (ir2 = ir1+1; ir2 != barMap.end(); ir2++)
    //         {
    //             checkBarLink(ir1, ir2);
    //         }
    //     }
    // }

    // void checkBarLink(BarNodeMap::iterator ir1, BarNodeMap::iterator ir2)
    // {
    //     // traverse the node of two region
    // }
    // 1. 将所有region放入一个list, 等待后续一个个挑出来
    // std::list<Word> regPool;
    // RegNodeMap::iterator ir;
    // for (ir = regMap.start(); ir != regMap.end(); ir++)
    // {
    //     regPool.push_back(ir->first);
    // }
