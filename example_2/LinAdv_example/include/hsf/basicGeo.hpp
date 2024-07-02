/*! \file basicGeo.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-03-23
 *  \modified 2023-12-20
 *  \brief basic standard shapes of 1/2/3 D
 */

#ifndef MESH_GEOMETRY_BASICGEO_HPP
#define MESH_GEOMETRY_BASICGEO_HPP
#include "integrationPoints.hpp"
#include "utilityType.h"

namespace HSF {

/*!
POINT       - a point
SEGMENT     - [0,1]
TRIANGLE    - (0,0), (1,0), (0,1)
SQUARE      - (0,1)x(0,1)
TETRAHEDRON - (0,0,0),(1,0,0),(0,1,0),(0,0,1)
CUBE        - (0,1)x(0,1)x(0,1)
PRISM       - (0,0,0),(0,1,0),(1,0,0),(0,0,1),(0,1,1),(1,0,1)
PYRAMID     - (0,0,0),(1,0,0),(1,1,0),(0,1,0),(0,0,1)
*/
class BasicGeometry {
 public:
  enum Type {
    INVALID = -1,
    POINT = 0,
    SEGMENT = 1,
    TRIANGLE = 2,
    SQUARE = 3,
    TETRAHEDRON = 4,
    CUBE = 5,
    PRISM = 6,
    PYRAMID = 7,
    NUMBASICGEOTYPE = 8 /*!< keep track num, easy-coding */
  };

  static const char *name[NUMBASICGEOTYPE];
  static const int maxDim = 3;
  static const int numGeom = NUMBASICGEOTYPE;
  static const scalar volume[NUMBASICGEOTYPE];
  static const int numBoundary[NUMBASICGEOTYPE];
  static const int numVerts[NUMBASICGEOTYPE];
  static const int numEdges[NUMBASICGEOTYPE];
  static const int numFaces[NUMBASICGEOTYPE];
  static const int dimension[NUMBASICGEOTYPE];
  static const int dimStart[maxDim + 2];

  template <Type T>
  struct Prop; /*!< property of one geometry */

 private:
  IntegrationPoints *vert_[NUMBASICGEOTYPE];
  WeightedPoint center_[NUMBASICGEOTYPE];

 public:
  /*! default constructor */
  BasicGeometry();
  /*! default deconstructor */
  ~BasicGeometry();

  /*! return true if the given  geom is of tensor-product type */
  static bool isTensorProduct(Type geom) {
    return geom == SEGMENT || geom == SQUARE || geom == CUBE;
  }
  /*! return type corresponding to a tensor-product of the given dimension. */
  static Type tensorProductGeometry(int dim) {
    switch (dim) {
      case 0:
        return POINT;
      case 1:
        return SEGMENT;
      case 2:
        return SQUARE;
      case 3:
        return CUBE;
      default:
        return INVALID;
    }
  }
  /*! return the number of boundary "faces" of a given BasicGeometry::Type.*/
  int numBdr(int geomType) { return numBoundary[geomType]; }
  /*! return all vertices of the given geom type. */
  const IntegrationPoints *getVertices(int geomType);
  /*! return the center of the given geom type. */
  const WeightedPoint &getCenter(int geomType) { return center_[geomType]; }
  /*! get a random point in the reference geom type */
  static void getRandomPoint(int geomType, WeightedPoint &ip);
  /*! Check if the given point is inside the given reference geom type.*/
  static bool checkPoint(int geomType, const WeightedPoint &ip);
  /*! check if the given point is inside the given geom type with the
   *  given tolerance eps*/
  static bool checkPoint(int geomType, const WeightedPoint &ip, double eps);
  /*! check if the end point is inside the geom type, if not overwrite it with
   * the point on the boundary that lies on the line segment between beg and end
   * (beg must be inside the geom). Return true if end is inside the geom, and
   * false otherwise. */
  static bool projectPoint(int geomType, const WeightedPoint &beg,
                           WeightedPoint &end);
  /*! If ip is outside the geom, replace it with the point on the boundary that
   * is closest to the original ip and return false;otherwise, return true
   * without changing ip. */
  static bool projectPoint(int geomType, WeightedPoint &ip);
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
template <>
struct BasicGeometry::Prop<BasicGeometry::POINT> {
  static const int dimension = 0;
  static const int numVert = 1;
  static const int numOrient = 1;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

template <>
struct BasicGeometry::Prop<BasicGeometry::SEGMENT> {
  static const int dimension = 1;
  static const int numVert = 2;
  static const int numEdges = 1;
  static const int edges[numEdges][2];

  static const int numOrient = 2;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

template <>
struct BasicGeometry::Prop<BasicGeometry::TRIANGLE> {
  static const int dimension = 2;
  static const int numVert = 3;
  static const int numEdges = 3;
  static const int edges[numEdges][2];
  /*! Upper-triangular part of the local vertex-to-vertex graph.*/
  /*!     0  1  2
   *   0 |  |o |o |
   *   1 |x |  |o |
   *   2 |x |x |  |
   */
  struct VertToVert {
    static const int I[numVert];
    static const int J[numEdges][2]; /*!< {end,edge_idx}*/
  };
  static const int numFaces = 1;
  static const int faceVert[numFaces][numVert];

  /*! For a given base tuple v={v0,v1,v2}, the orientation of a permutation
   * u={u0,u1,u2} of v, is an index 'j' such that u[i]=v[orient[j][i]]. The
   * static method Mesh::GetTriOrientation, computes the index 'j' of the
   * permutation that maps the second argument 'test' to the first argument
   * 'base': test[orient[j][i]]=base[i]. */
  static const int numOrient = 6;
  static const int orient[numOrient][numVert];
  /*! The inverse of orientation 'j' is invOrient[j].
   * k=invOrient[i] such that u[orient[k][i]]=v[i], */
  static const int invOrient[numOrient];
};

template <>
struct BasicGeometry::Prop<BasicGeometry::SQUARE> {
  static const int dimension = 2;
  static const int numVert = 4;
  static const int numEdges = 4;
  static const int edges[numEdges][2];

  /*!     0  1  2  3
   *   0 |  |o |  |o |
   *   1 |x |  |o |  |
   *   2 |  |x |  |o |
   *   3 |x |  |x |  |
   */
  struct VertToVert {
    static const int I[numVert];
    static const int J[numEdges][2];
  };
  static const int numFaces = 1;
  static const int faceVert[numFaces][numVert];

  static const int numOrient = 8;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

/*!     0  1  2  3
 *   0 |  |o |o |o |
 *   1 |x |  |o |o |
 *   2 |x |x |  |o |
 *   3 |x |x |x |  |
 */
template <>
struct BasicGeometry::Prop<BasicGeometry::TETRAHEDRON> {
  static const int dimension = 3;
  static const int numVert = 4;
  static const int numEdges = 6;
  static const int edges[numEdges][2];
  static const int numFaces = 4;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 3;
  static const int faceVert[numFaces][maxFaceVert];

  struct VertToVert {
    static const int I[numVert];
    static const int J[numEdges][2];
  };

  static const int numOrient = 24;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

/*!     0  1  2  3  4  5  6  7
 *   0 |  |o |  |o |o |  |  |  |
 *   1 |x |  |o |  |  |o |  |  |
 *   2 |  |x |  |o |  |  |o |  |
 *   3 |x |  |x |  |  |  |  |o |
 *   4 |x |  |  |  |  |o |  |o |
 *   5 |  |x |  |  |x |  |o |  |
 *   6 |  |  |x |  |  |x |  |o |
 *   7 |  |  |  |x |x |  |x |  |
 */
template <>
struct BasicGeometry::Prop<BasicGeometry::CUBE> {
  static const int dimension = 3;
  static const int numVert = 8;
  static const int numEdges = 12;
  static const int edges[numEdges][2];
  static const int numFaces = 6;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 4;
  static const int faceVert[numFaces][maxFaceVert];

  struct VertToVert {
    static const int I[numVert];
    static const int J[numEdges][2];
  };
};

/*!     0  1  2  3  4  5
 *   0 |  |o |o |o |  |  |
 *   1 |x |  |o |  |o |  |
 *   2 |x |x |  |  |  |o |
 *   3 |x |  |  |  |o |o |
 *   4 |  |x |  |x |  |o |
 *   5 |  |  |x |x |x |  |
 */
template <>
struct BasicGeometry::Prop<BasicGeometry::PRISM> {
  static const int dimension = 3;
  static const int numVert = 6;
  static const int numEdges = 9;
  static const int edges[numEdges][2];
  static const int numFaces = 5;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 4;
  static const int faceVert[numFaces][maxFaceVert];

  struct VertToVert {
    static const int I[numVert];
    static const int J[numEdges][2];
  };
};

/*!     0  1  2  3  4
 *   0 |  |o |  |o |o |
 *   1 |x |  |o |  |o |
 *   2 |  |x |  |o |o |
 *   3 |x |  |x |  |o |
 *   4 |x |x |x |x |  |
 */
template <>
struct BasicGeometry::Prop<BasicGeometry::PYRAMID> {
  static const int dimension = 3;
  static const int numVert = 5;
  static const int numEdges = 8;
  static const int edges[numEdges][2];
  static const int numFaces = 5;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 4;
  static const int faceVert[numFaces][maxFaceVert];

  struct VertToVert {
    static const int I[numVert];
    static const int J[numEdges][2];
  };
};

extern BasicGeometry BasicGeometries;

} /* End namespace HSF */

#endif /* end of include guard: MESH_GEOMETRY_BASICGEO_HPP */
