/*! \file H1element.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-10-18
 *  \brief H1 space finite element
 */

#ifndef SRC_FEM_H1ELEMENTS_HPP
#define SRC_FEM_H1ELEMENTS_HPP

#include "finiteElement.hpp"

namespace HSF {

class H1FiniteElement : public FiniteElement {
 protected:
  std::vector<int> ordering_;
  Basis1D *basis1d_;

 public:
  H1FiniteElement(int d, Geometry::Type g, int dof, int order,
                  int f = FunctionSpace::Pk);
  virtual ~H1FiniteElement();
  /*! @brief Get an std::vector<int> that maps lexicographically ordered indices to
    the indices of the respective nodes/dofs/basis functions.
    Lexicographic ordering of nodes is defined in terms of reference-space
    coordinates (x,y,z). Lexicographically ordered nodes are listed first in
    order of increasing x-coordinate, and then in order of increasing
    y-coordinate, and finally in order of increasing z-coordinate.
   */
  const std::vector<int> &getOrdering() const { return ordering_; }
  /*! for scalar finite elment */
  /*! @brief Evaluate the values of all shape functions of a scalar finite
      element in reference space at the given point @a ip. */
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const = 0;
  /*! @brief Evaluate the gradients of all shape functions of a scalar finite
      element in reference space at the given point @a ip. */
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const = 0;
};
/*! 0D */
class PointElement : public FiniteElement {
 public:
  PointElement();
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const;
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const;
};

/*! * * * * * * * * * * * Arbitrary order H1 elements * * * * * * * * * */
class H1Segment : public H1FiniteElement {
 private:
  mutable std::vector<scalar> shape_, dshape_;

 public:
  H1Segment(const int p, const int btype = BasisType::LagrangeLobatto);
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const;
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const;
};

/*! 基函数求法比较特别 */
class H1Triangle : public H1FiniteElement {
 private:
  mutable std::vector<scalar> shape_x_, shape_y_, shape_l_, dshape_x_, dshape_y_,
      dshape_l_, u_;
  mutable DenseMatrix<scalar> du_;
  DenseMatrix<scalar> T_;
  std::vector<int> piv_;

 public:
  H1Triangle(const int p, const int btype = BasisType::LagrangeLobatto);
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const;
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const;
};

/*! tensor product basis */
class H1Quadrilateral : public H1FiniteElement {
 private:
  mutable std::vector<scalar> shape_x_, shape_y_, dshape_x_, dshape_y_;

 public:
  H1Quadrilateral(const int p, const int btype = BasisType::LagrangeLobatto);
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const;
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const;
};

/*! 基函数求法比较特别 */
class H1Tetrahedron : public H1FiniteElement {
 private:
  mutable std::vector<scalar> shape_x_, shape_y_, shape_z_, shape_l_, dshape_x_,
      dshape_y_, dshape_z_, dshape_l_, u_;
  mutable DenseMatrix<scalar> du_;
  DenseMatrix<scalar> T_;
  std::vector<int> piv_;

 public:
  H1Tetrahedron(const int p, const int btype = BasisType::LagrangeLobatto);
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const;
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const;
};

class H1Wedge : public H1FiniteElement {
 private:
  std::vector<int> tri_dof_, seg_dof_;
  H1Triangle tri_;
  H1Segment seg_;
  mutable Vector<scalar> tri_shape_, seg_shape_;
  mutable DenseMatrix<scalar> tri_dshape_, seg_dshape_;

 public:
  H1Wedge(const int p, const int btype = BasisType::LagrangeLobatto);
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const;
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const;
};

/*! tensor product basis */
class H1Hexahedron : public H1FiniteElement {
 private:
  mutable std::vector<scalar> shape_x_, shape_y_, shape_z_, dshape_x_, dshape_y_,
      dshape_z_;

 public:
  H1Hexahedron(const int p, const int btype = BasisType::LagrangeLobatto);
  virtual void evalShape(const Tensor1<scalar> &ip,
                         Vector<scalar> &shape) const;
  virtual void evalDShape(const Tensor1<scalar> &ip,
                          DenseMatrix<scalar> &dshape) const;
};

/*! * * * * * * * * * * * Fixed order H1 elements * * * * * * * * * */
#define ELEMENTDEFINITION(name)                                 \
  class name : public FiniteElement {                           \
   public:                                                      \
    name();                                                     \
    virtual void evalShape(const Tensor1<scalar> &ip,           \
                           Vector<scalar> &shape) const;        \
    virtual void evalDShape(const Tensor1<scalar> &ip,          \
                            DenseMatrix<scalar> &dshape) const; \
  };

/*! 1D segement */
ELEMENTDEFINITION(H1N2Segment); /*!< linear */
ELEMENTDEFINITION(H1N3Segment); /*!< quadratic */
ELEMENTDEFINITION(H1N4Segment); /*!< cubic */
/*! 2D triangle */
ELEMENTDEFINITION(H1N3Triangle);  /*!< linear */
ELEMENTDEFINITION(H1N6Triangle);  /*!< quadratic */
ELEMENTDEFINITION(H1N10Triangle); /*!< cubic */
/*! 2D Quadrilateral */
ELEMENTDEFINITION(H1N4Quadrilateral);  /*!< bi-linear */
ELEMENTDEFINITION(H1N9Quadrilateral);  /*!< bi-quadratic */
ELEMENTDEFINITION(H1N16Quadrilateral); /*!< bi-cubic */
/*! 3D Tetrahedron */
ELEMENTDEFINITION(H1N4Tetrahedron);  /*!< linear */
ELEMENTDEFINITION(H1N10Tetrahedron); /*!< quadratic */
ELEMENTDEFINITION(H1N20Tetrahedron); /*!< cubic */
/*! 3D Wedge */
ELEMENTDEFINITION(H1N6Wedge);  /*!< linear */
ELEMENTDEFINITION(H1N18Wedge); /*!< quadratic */
ELEMENTDEFINITION(H1N40Wedge); /*!< cubic */
/*! 3D Hexahedron */
ELEMENTDEFINITION(H1N8Hexahedron);  /*!< tri-linear */
ELEMENTDEFINITION(H1N27Hexahedron); /*!< tri-quadratic */
ELEMENTDEFINITION(H1N64Hexahedron); /*!< tri-cubic */
/*! 3D Pyramid */
ELEMENTDEFINITION(H1N5Pyramid);  /*!< linear */
ELEMENTDEFINITION(H1N13Pyramid); /*!< quadratic */

#undef ELEMENTDEFINITION
} /* End namespace HSF */
#endif /* end of include guard: SRC_FEM_H1ELEMENTS_HPP */
