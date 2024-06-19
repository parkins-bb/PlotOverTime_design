#ifndef SOURCE_WRAPPER_HSFPATCHDATA_H
#define SOURCE_WRAPPER_HSFPATCHDATA_H
/*! \file PatchData.h
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \brief brief description
 */

#include <cstdio>

#include "EntityUtilities.h"
#include "hsfField.hpp"

namespace HSF {

namespace hier {
// template <int DIM, class TYPE>
// class ArrayData;

template <int DIM>
class PatchData {
 public:
  PatchData() {}
  // PatchData(EntityUtilities::PatchDataType type, const int entity) {}
  // PatchData(EntityUtilities::PatchDataType type, const int group, const int
  // entity, const int depth = 1) {}
  virtual ~PatchData() {}

  // int getGroup() const {}
  // int getDepth() const {}
 protected:
  EntityUtilities::PatchDataType d_type;
};

}  // namespace hier

namespace pdat {

template <int DIM, class TYPE>
class CellData : public hier::PatchData<DIM> {
 public:
  CellData(Field<TYPE>* f) : hier::PatchData<DIM>() { d_f = f; }
  // CellData(const int group, const int entity, const int depth = 1) {}
  // CellData(const CellData<DIM, TYPE>& cell_data) {}
  virtual ~CellData() {}
  TYPE* getPointer(const int d = 0) {
    return reinterpret_cast<TYPE*>(d_f->data());
  }
  const TYPE* getPointer(const int d = 0) const {
    return reinterpret_cast<TYPE*>(d_f->data());
  }

  TYPE& operator()(const int g, const int i, const int d = 0) {
    return (*d_f)(i, g);
  }
  const TYPE& operator()(const int g, const int i, const int d = 0) const {
    return (*d_f)(i, g);
  }

  // ArrayData<DIM, TYPE>& getArrayData() {}
  // const ArrayData<DIM, TYPE>& getArrayData() const {}

  void fill(const TYPE& t, const int d = 0) { (*d_f) = t; }
  void fillAll(const TYPE& t) { (*d_f) = t; }

  void setTime(const double timestamp) { d_f->setTime(timestamp); }
  double getTime() const { return d_f->getTime(); }
  // EntityUtilities::PatchDataType getType() const {}
  int getNumberOfEntity() const { return d_f->getEntityNum(); }

  virtual void fill(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) = t;
  }

  virtual void add(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) += t;
  }

 private:
  Field<TYPE>* d_f;
};
template <int DIM, class TYPE>
class FaceData : public hier::PatchData<DIM> {
 public:
  FaceData(Field<TYPE>* f) : hier::PatchData<DIM>() { d_f = f; }
  // FaceData(const int group, const int entity, const int depth = 1) {}
  // FaceData(const FaceData<DIM, TYPE>& cell_data) {}
  TYPE* getPointer(const int d = 0) {
    return reinterpret_cast<TYPE*>(d_f->data());
  }
  const TYPE* getPointer(const int d = 0) const {
    return reinterpret_cast<TYPE*>(d_f->data());
  }

  TYPE& operator()(const int g, const int i, const int d = 0) {
    return (*d_f)(i, g);
  }
  const TYPE& operator()(const int g, const int i, const int d = 0) const {
    return (*d_f)(i, g);
  }

  // ArrayData<DIM, TYPE>& getArrayData() {}
  // const ArrayData<DIM, TYPE>& getArrayData() const {}

  void fill(const TYPE& t, const int d = 0) { (*d_f) = t; }
  void fillAll(const TYPE& t) { (*d_f) = t; }

  void setTime(const double timestamp) { d_f->setTime(timestamp); }
  double getTime() const { return d_f->getTime(); }
  // EntityUtilities::PatchDataType getType() const {}
  int getNumberOfEntity() const { return d_f->getEntityNum(); }

  virtual void fill(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) = t;
  }

  virtual void add(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) += t;
  }

  virtual ~FaceData() {}

 private:
  Field<TYPE>* d_f;
};
template <int DIM, class TYPE>
class EdgeData : public hier::PatchData<DIM> {
 public:
  EdgeData(Field<TYPE>* f) : hier::PatchData<DIM>() { d_f = f; }
  // EdgeData(const int group, const int entity, const int depth = 1) {}
  // EdgeData(const EdgeData<DIM, TYPE>& cell_data) {}
  TYPE* getPointer(const int d = 0) {
    return reinterpret_cast<TYPE*>(d_f->data());
  }
  const TYPE* getPointer(const int d = 0) const {
    return reinterpret_cast<TYPE*>(d_f->data());
  }

  TYPE& operator()(const int g, const int i, const int d = 0) {
    return (*d_f)(i, g);
  }
  const TYPE& operator()(const int g, const int i, const int d = 0) const {
    return (*d_f)(i, g);
  }

  // ArrayData<DIM, TYPE>& getArrayData() {}
  // const ArrayData<DIM, TYPE>& getArrayData() const {}

  void fill(const TYPE& t, const int d = 0) { (*d_f) = t; }
  void fillAll(const TYPE& t) { (*d_f) = t; }

  void setTime(const double timestamp) { d_f->setTime(timestamp); }
  double getTime() const { return d_f->getTime(); }
  // EntityUtilities::PatchDataType getType() const {}
  int getNumberOfEntity() const { return d_f->getEntityNum(); }

  virtual void fill(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) = t;
  }

  virtual void add(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) += t;
  }

  virtual ~EdgeData() {}

 private:
  Field<TYPE>* d_f;
};
template <int DIM, class TYPE>
class NodeData : public hier::PatchData<DIM> {
 public:
  NodeData(Field<TYPE>* f) : hier::PatchData<DIM>() { d_f = f; }
  // NodeData(const int group, const int entity, const int depth = 1) {}
  // NodeData(const NodeData<DIM, TYPE>& cell_data) {}
  TYPE* getPointer(const int d = 0) {
    return reinterpret_cast<TYPE*>(d_f->data());
  }
  const TYPE* getPointer(const int d = 0) const {
    return reinterpret_cast<TYPE*>(d_f->data());
  }

  TYPE& operator()(const int g, const int i, const int d = 0) {
    return (*d_f)(i, g);
  }
  const TYPE& operator()(const int g, const int i, const int d = 0) const {
    return (*d_f)(i, g);
  }

  // ArrayData<DIM, TYPE>& getArrayData() {}
  // const ArrayData<DIM, TYPE>& getArrayData() const {}

  void fill(const TYPE& t, const int d = 0) { (*d_f) = t; }
  void fillAll(const TYPE& t) { (*d_f) = t; }

  void setTime(const double timestamp) { d_f->setTime(timestamp); }
  double getTime() const { return d_f->getTime(); }
  // EntityUtilities::PatchDataType getType() const {}
  int getNumberOfEntity() const { return d_f->getEntityNum(); }

  virtual void fill(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) = t;
  }

  virtual void add(const TYPE t, const int g, const int i, const int d) {
    (*d_f)(i, g) += t;
  }

  virtual ~NodeData() {}

 private:
  Field<TYPE>* d_f;
};

/*! bool类型特殊处理 */
#define INTTYPEDATA                                                        \
 public:                                                                   \
  int* getPointer(const int d = 0) {                                       \
    return reinterpret_cast<int*>(d_f->data());                            \
  }                                                                        \
  const int* getPointer(const int d = 0) const {                           \
    return reinterpret_cast<int*>(d_f->data());                            \
  }                                                                        \
                                                                           \
  int& operator()(const int g, const int i, const int d = 0) {             \
    return (*d_f)(i, g);                                                   \
  }                                                                        \
  const int& operator()(const int g, const int i, const int d = 0) const { \
    return (*d_f)(i, g);                                                   \
  }                                                                        \
                                                                           \
  void fill(const bool& t, const int d = 0) { (*d_f) = t; }                \
  void fillAll(const bool& t) { (*d_f) = t; }                              \
  void setTime(const double timestamp) { d_f->setTime(timestamp); }        \
  double getTime() const { return d_f->getTime(); }                        \
  int getNumberOfEntity() const { return d_f->getEntityNum(); }            \
  virtual void fill(const bool t, const int g, const int i, const int d) { \
    (*d_f)(i, g) = t;                                                      \
  }                                                                        \
                                                                           \
  virtual void add(const bool t, const int g, const int i, const int d) {  \
    (*d_f)(i, g) += t;                                                     \
  }

template <int DIM>
class CellData<DIM, bool> : public hier::PatchData<DIM> {
 public:
  CellData(Field<int>* f) : hier::PatchData<DIM>() { d_f = f; }
  virtual ~CellData() {}

 private:
  Field<int>* d_f;
  // FaceData(const int group, const int entity, const int depth = 1) {}
  // FaceData(const FaceData<DIM, TYPE>& cell_data) {}
  INTTYPEDATA
};
template <int DIM>
class FaceData<DIM, bool> : public hier::PatchData<DIM> {
 public:
  FaceData(Field<int>* f) : hier::PatchData<DIM>() { d_f = f; }
  virtual ~FaceData() {}

 private:
  Field<int>* d_f;
  // FaceData(const int group, const int entity, const int depth = 1) {}
  // FaceData(const FaceData<DIM, TYPE>& cell_data) {}
  INTTYPEDATA
};
template <int DIM>
class EdgeData<DIM, bool> : public hier::PatchData<DIM> {
 public:
  EdgeData(Field<int>* f) : hier::PatchData<DIM>() { d_f = f; }
  virtual ~EdgeData() {}

 private:
  Field<int>* d_f;
  // FaceData(const int group, const int entity, const int depth = 1) {}
  // FaceData(const FaceData<DIM, TYPE>& cell_data) {}
  INTTYPEDATA
};
template <int DIM>
class NodeData<DIM, bool> : public hier::PatchData<DIM> {
 public:
  NodeData(Field<int>* f) : hier::PatchData<DIM>() { d_f = f; }
  virtual ~NodeData() {}

 private:
  Field<int>* d_f;
  // FaceData(const int group, const int entity, const int depth = 1) {}
  // FaceData(const FaceData<DIM, TYPE>& cell_data) {}
  INTTYPEDATA
};

}  // namespace pdat
}  // namespace HSF

#endif /* end of include guard: SOURCE_WRAPPER_HSFPATCHDATA_H */
