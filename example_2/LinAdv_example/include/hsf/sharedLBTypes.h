/**
 * @Author:   Gao Fei
 * @Date:     2022-02-02 10:47:33
 * @E-mail:   gaofei@mail.nsccwx.cn
 * @Project:  hsf_edit
 * @FileName: sharedLBTypes.h
 *
 * @Last Modified by:   gaofei
 * @Last Modified time: 2022-04-13 13:59:16
 *  
 * @Description: 
 *  
 */

#ifndef sharedLBTypes_H
#define sharedLBTypes_H

// include
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <boost/assert.hpp>
#include "mpi.h"

#define UNIT_TEST
#ifndef UNIT_TEST
#include "utilities.h"
#else
#include "dummyUtilities.h"
#endif

using namespace std;

/**
 * @brief      calculation or synchronic
 */
enum class NodeType
{
    calcOpt = 0,
    syncOpt = 1
};

/**
 * @brief      for synchronic node, synchron with which region and which
 *             operation
 */
struct SyncInfo
{
    Word regName;
    Word optName;
};

/**
 * @brief      node in a procedure graph
 *
 * @details    SyncInfo is enabled only when NodeType is syncOpt
 */
struct RegNode 
{
    Word      name;
    NodeType  type;
    scalar    load;
    SyncInfo  sync;
};

struct Parameter
{
    label resourceNum;
};

#endif


/*class edit module
#ifndef 
#define

class xxx
    public: xxx
{
    // - core member data

public:
    // - constructors and destructor

    // - member functions operations

    // - member functions access

private:
    // - member functions relization

    // - member data relization

};

#endif


#if 1 // debug
{
    printf("\nDEGUG in %s: %s, line %d\n",__FILE__, __FUNCTION__, __LINE__);
}
#endif


*/
/* ***********************************  old code ***********************************/
// class RegNode {
//     Word name;
//     RegionOpt* regOpt;
//     scalar load;
// };

// class BarNode {
//     Word name;
//     BarrierOpt* barOpt;
//     scalar load;
//     Word syncReg;
//     Word syncName;
// };

// /**
//      * @brief region name, operation name, node
//      */
//     typedef Table<Word, Table<Word, RegNode> > RegNodeMap;

//     /**
//      * @brief region name, barrier name, nide 
//      */
//     typedef Table<Word, Table<Word, BarNode> > BarNodeMap;

//     RegNodeMap regMap;
//     BarNodeMap barMap;
/* ********************************  end old code **********************************/




