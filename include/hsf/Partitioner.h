/**
 * @Author:   Gao Fei
 * @Date:     2022-03-09 10:55:43
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: Partitioner.h
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-04-02 12:00:49
 *  
 * @Description: 
 *  
 */

#ifndef Partitioner_H
#define Partitioner_H

#include "sharedLBTypes.h"
#include "ProcedureGraph.h"

/**
 * @brief      partition procedure graph
 */
class Partitioner
{
    // - core data
    ProcedureGraph& pg;

    std::set<Word> indpGroup;
    std::vector<std::set<Word> > bundGroups;
    scalar indpRegsLoad;
    scalar bundRegsLoad;
    Table<Word, scalar> regLoads;

    label indpResrc, bundResrc;

    Table<Word, label> indpRegAssign;
    Table<Word, label> bundRegAssign;

public:
    // - constructors
    Partitioner(ProcedureGraph& pg);

    // - destructor
    ~Partitioner(){};

    // - member functions
    void process();

    // - member function access
    Table<Word, label>& getIndpAssign()
    {
        return indpRegAssign;
    }

    Table<Word, label>& getbundAssign()
    {
        return bundRegAssign;
    }

    std::vector<std::set<Word> >& getBundGroup()
    {
        return bundGroups;
    }

private:
    // - relization functions
    void groupRegions();
    void statisticsLoad();
    void assignIndepend();
    void assignBundled();
    void correctAssign();
    void checkPutBundleGroups(Word reg1, Word reg2);
};


/**
 * @brief      for SAMR-LBM
 */
// class PartitionerRecursive
//     public: Partitioner
// {
//     // - core member data
//     // input
//     ProcedureGraph& pg;

//     // output
//     Table<Word, label> indpRegAssign;
//     Table<Word, label> bundRegAssign;

// public:
//     // - constructor and destructor

//     // - member functions operations
//     void process();

//     // - member functions access

// private:
//     // - member functions relization

//     // - member data relization

// };


#endif
