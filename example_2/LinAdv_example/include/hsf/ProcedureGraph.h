/**
 * @Author:   Gao Fei
 * @Date:     2022-02-22 11:32:56
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: ProcedureGraph.h
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-04-02 12:01:37
 *  
 * @Description: 
 *  
 */

#ifndef ProcedureGraph_H
#define ProcedureGraph_H

#include "sharedLBTypes.h"

// type define
typedef Table<Word, RegNode> RegProcedure;
typedef Table<Word, RegProcedure > RegNodeMap;

/**
 * @brief      input for Scheduler
 *
 * @details    register region name, not any region is legal
 */
class ProcedureGraph
{
    // - core datas
    // region name, opt name, node
    RegNodeMap regMap;

public:
    // - constructor
    ProcedureGraph(){}

    // - destrutor
    ~ProcedureGraph(){}

    // - member functions data operations
    void addRegNode(Word ir, Word iop, scalar load);

    void addBarNode(Word ir, Word iop, scalar load);

    void mergeNode(Word ir1, Word iop1, Word ir2, Word iop2);

    void deleteRegNode(){};
    void deleteBarNode(){};

    // - member functionsdata access
    RegNodeMap& getRegNode()
    {
        return regMap;
    }
};


#endif
