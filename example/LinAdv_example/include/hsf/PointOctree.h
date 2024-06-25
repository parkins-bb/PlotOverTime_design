//
// 文件名:	PointOctree.h
// 软件包:
// 版权  :	(c) 2004-2014 北京应用物理与计算数学研究所
// 版本号:	$Revision$
// 修改  :	$Date$
// 描述  :
//

#ifndef included_tbox_PointOctree
#define included_tbox_PointOctree

#include <iostream>
#include <vector>

namespace HSF {
namespace tbox {

/*!
 * @brief 点八叉树类 PointOctree 用于组织以点坐标作为索引的数据，实现高效查询。
 *
 * 该数据结构适应于任意空间维数：
 *  - PointOctree<1, TYPE> 等价于二叉树。
 *  - PointOctree<2, TYPE> 等价于四叉树。
 *  - PointOctree<3, TYPE> 等价于八叉树。
 *
 */
template <int DIM, class TYPE>
class PointOctree {
 private:
  struct NodeData;
  struct Node;

 public:
  /**
   * @brief 构造函数。
   * @param bounding_box 输入参数，二维浮点数组，八叉树空间区域。
   * @param node_capacity 输入参数，整型，树中每个结点容量。
   */
  PointOctree(double bounding_box[DIM][2], int node_capacity = 8) {
    d_root = new Node(bounding_box, node_capacity, 0);
  }

  /**
   * @brief 析构函数。
   */
  ~PointOctree() { delete d_root; }

  /**
   * @brief 插入一个数据。
   * @param coord 输入参数，浮点数组，空间坐标。
   * @param data  输入参数，TYPE型，被坐标‘coord’索引的数据。
   * @return 插入是否成功。
   * @note   若坐标‘coord’在八叉树的空间区域之外，则插入失败，返回FALSE。
   */
  bool insert(double coord[DIM], TYPE data) {
    return d_root->insert(coord, data);
  }

  /**
   * @brief 查找数据。
   * @param coord 输入参数，浮点数组，空间坐标。
   * @param data
   * 输出参数，TYPE型数组，存储八叉树中与‘coord’重合的点所索引的数据。
   * @param tol   输入参数，浮点数，容差，若A点和B点满足 |A-B|<=tol，
   * 则认为它们重合。
   */
  void search(double coord[DIM], std::vector<TYPE>& data,
              double tol = 1.0e-12) {
    data.resize(0);
    return d_root->search(coord, data, tol);
  }

  /**
   * @brief 查找数据。
   * @param coord 输入参数，二维浮点数组，被查找的空间区域。
   * @param data 输出参数，TYPE型数组，存储八叉树中落在‘box’内的点所索引的数据。
   */
  void search(double box[DIM][2], std::vector<TYPE>& data) {
    data.resize(0);
    return d_root->search(box, data);
  }

  /**
   * @brief 查找数据。
   * @param coord 输入参数，浮点数组，空间坐标。
   * @param tol   输入参数，浮点数，容差，若A点和B点满足 |A-B|<=tol，
   * 则认为它们重合。
   * @reture      布尔值，若找到至少一个元素，则返回真，否则返回假。
   */
  bool find(double coord[DIM], double tol = 1.0e-12) {
    return d_root->find(coord, tol);
  }

  /**
   * @brief 查找数据。
   * @param coord 输入参数，二维浮点数组，被查找的空间区域。
   * @reture      布尔值，若找到至少一个元素，则返回真，否则返回假。
   */
  bool find(double box[DIM][2]) { return d_root->find(box); }

  /**
   * @brief 树中存储的点数。
   */
  int size() const { return d_root->size(); }

  /**
   * @brief 树的深度。
   * @note  该函数仅用于性能分析与调优。
   */
  int depth() const { return d_root->depth(); }

  /**
   * @brief 树的结点数。
   * @note  该函数仅用于性能分析与调优。
   */
  int nodes() const { return d_root->nodes(); }

  /**
   * @brief 打印。
   * @note  该函数仅用于性能分析与调优。
   */
  void printClassData(std::ostream& os) const {
    os << "PointOctree:\n";
    os << "  size " << size() << "\n";
    os << "  depth " << depth() << "\n";
    os << "  nodes " << nodes() << "\n";
    d_root->printClassData(os, true);
  }

 private:
  Node* d_root;

 private:
  struct NodeData {
    double d_coord[DIM];
    TYPE d_data;
  };

  struct Node {
    int d_count;
    int d_capacity;
    int d_level;
    double d_bounding_box[DIM][2];

    NodeData* d_data;

    /*
     * 2 | 3
     * -----
     * 0 | 1
     */
    Node* d_children[1 << DIM];

    Node(double bounding_box[DIM][2], int capacity, int level)
        : d_count(0), d_capacity(capacity), d_level(level) {
      for (int d = 0; d < DIM; d++) {
        d_bounding_box[d][0] = bounding_box[d][0];
        d_bounding_box[d][1] = bounding_box[d][1];
      }
      for (int c = 0; c < (1 << DIM); c++) d_children[c] = 0;
      d_data = new NodeData[d_capacity];
    }

    ~Node() {
      if (d_data != 0) delete[] d_data;

      int nchild = 1 << DIM;
      for (int c = 0; c < nchild; c++)
        if (d_children[c] != 0) delete d_children[c];
    }

    void printClassData(std::ostream& os, bool recursive) const {
      for (int s = 0; s < d_level; s++) os << "  ";
      os << "  Node [lb";
      for (int d = 0; d < DIM; d++) os << " " << d_bounding_box[d][0];
      os << ", rt";
      for (int d = 0; d < DIM; d++) os << " " << d_bounding_box[d][1];
      os << " ]: ";
      os << "size " << d_count << std::endl;

      if (recursive) {
        for (int c = 0; c < (1 << DIM); c++)
          if (d_children[c] != 0) d_children[c]->printClassData(os, recursive);
      }
    }

    int size() const {
      int cnt = d_count;
      for (int c = 0; c < (1 << DIM); c++)
        if (d_children[c] != 0) cnt += d_children[c]->size();
      return cnt;
    }

    int depth() const {
      int max_child_depth = 0;
      for (int c = 0; c < (1 << DIM); c++) {
        if (d_children[c] != 0) {
          int child_depth = d_children[c]->depth();
          if (max_child_depth < child_depth) max_child_depth = child_depth;
        }
      }
      return max_child_depth + 1;
    }

    int nodes() const {
      int num = 0;
      for (int c = 0; c < (1 << DIM); c++) {
        if (d_children[c] != 0) num += d_children[c]->nodes();
      }
      return num + 1;
    }

    Node* createChild(int which) {
      double center[DIM];
      for (int d = 0; d < DIM; d++)
        center[d] = 0.5 * (d_bounding_box[d][0] + d_bounding_box[d][1]);

      double box[DIM][2];
      for (int d = 0; d < DIM; d++) {
        int part = ((1 << d) & which) ? 1 : 0;
        box[d][0] = part == 0 ? d_bounding_box[d][0] : center[d];
        box[d][1] = part == 1 ? d_bounding_box[d][1] : center[d];
      }

      d_children[which] = new Node(box, d_capacity, d_level + 1);
      return d_children[which];
    }

    bool insert(double coord[DIM], TYPE data) {
      // points outside the bounding box will be dropped
      for (int d = 0; d < DIM; d++)
        if (coord[d] < d_bounding_box[d][0] || coord[d] > d_bounding_box[d][1])
          return false;

      if (d_count < d_capacity) {
        // capacity not full, insert to this node
        for (int d = 0; d < DIM; d++) d_data[d_count].d_coord[d] = coord[d];
        d_data[d_count].d_data = data;
        d_count++;
        return true;
      }

      // insert to child
      int which_child = 0;
      for (int d = 0; d < DIM; d++) {
        double center = 0.5 * (d_bounding_box[d][0] + d_bounding_box[d][1]);
        int part = (coord[d] <= center) ? 0 : 1;
        part = part << d;
        which_child += part;
      }

      if (d_children[which_child] == 0)
        d_children[which_child] = createChild(which_child);

      return d_children[which_child]->insert(coord, data);
    }

    void search(double coord[DIM], std::vector<TYPE>& data, double tol) {
      if (d_count == 0) return;

      for (int d = 0; d < DIM; d++) {
        if (coord[d] < d_bounding_box[d][0] - tol ||
            coord[d] > d_bounding_box[d][1] + tol)
          return;
      }

      for (int n = 0; n < d_count; n++) {
        bool match = true;
        for (int d = 0; d < DIM; d++) {
          if (coord[d] - d_data[n].d_coord[d] > tol ||
              d_data[n].d_coord[d] - coord[d] > tol) {
            match = false;
            break;
          }
        }

        if (match) data.push_back(d_data[n].d_data);
      }

      int nchild = 1 << DIM;
      for (int c = 0; c < nchild; c++)
        if (d_children[c] != 0) d_children[c]->search(coord, data, tol);
    }

    void search(double box[DIM][2], std::vector<TYPE>& data) {
      if (d_count == 0) return;

      for (int d = 0; d < DIM; d++) {
        if (box[d][1] < d_bounding_box[d][0] ||
            box[d][0] > d_bounding_box[d][1])
          return;
      }

      for (int n = 0; n < d_count; n++) {
        bool match = true;
        for (int d = 0; d < DIM; d++) {
          if (box[d][0] > d_data[n].d_coord[d] ||
              d_data[n].d_coord[d] > box[d][1]) {
            match = false;
            break;
          }
        }

        if (match) data.push_back(d_data[n].d_data);
      }

      int nchild = 1 << DIM;
      for (int c = 0; c < nchild; c++)
        if (d_children[c] != 0) d_children[c]->search(box, data);
    }

    bool find(double coord[DIM], double tol) {
      if (d_count == 0) return false;

      for (int d = 0; d < DIM; d++) {
        if (coord[d] < d_bounding_box[d][0] - tol ||
            coord[d] > d_bounding_box[d][1] + tol)
          return false;
      }

      for (int n = 0; n < d_count; n++) {
        bool match = true;
        for (int d = 0; d < DIM; d++) {
          if (coord[d] - d_data[n].d_coord[d] > tol ||
              d_data[n].d_coord[d] - coord[d] > tol) {
            match = false;
            break;
          }
        }

        if (match) return true;
      }

      int nchild = 1 << DIM;
      for (int c = 0; c < nchild; c++)
        if (d_children[c] != 0)
          if (d_children[c]->find(coord, tol)) return true;

      return false;
    }

    bool find(double box[DIM][2]) {
      if (d_count == 0) return false;

      for (int d = 0; d < DIM; d++) {
        if (box[d][1] < d_bounding_box[d][0] ||
            box[d][0] > d_bounding_box[d][1])
          return false;
      }

      for (int n = 0; n < d_count; n++) {
        bool match = true;
        for (int d = 0; d < DIM; d++) {
          if (box[d][0] > d_data[n].d_coord[d] ||
              d_data[n].d_coord[d] > box[d][1]) {
            match = false;
            break;
          }
        }

        if (match) return true;
      }

      int nchild = 1 << DIM;
      for (int c = 0; c < nchild; c++)
        if (d_children[c] != 0)
          if (d_children[c]->find(box)) return true;

      return false;
    }
  };
};

}  // namespace tbox
}  // namespace HSF

#endif  // included_tbox_PointOctree
