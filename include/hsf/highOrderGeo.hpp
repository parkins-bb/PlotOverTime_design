/*! \file highOrderGeo.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2023-03-23
 *  \modified 2023-12-29
 *  \brief high order shapes' prop standard
 */

#ifndef MESH_GEOMETRY_HIGHORDERGEO_HPP
#define MESH_GEOMETRY_HIGHORDERGEO_HPP

#include "basicGeo.hpp"
#include "xdmfTool.hpp"

namespace HSF {

/*! factory */
class GetGeomProp {
 public:
  static int dimension(int type);
  static int numVert(int type);
  static int numEdges(int type);
  static int maxEdgeVert(int type);
  static const int *edges(int type); /*!< 2D array */
  static int numFaces(int type);
  static const int *faceTypes(int type);
  static int maxFaceVert(int type);
  static const int *faceVert(int type); /*!< 2D array */
  static int numOrient(int type);
  static const int *orient(int type); /*!< 2D array */
  static const int *invOrient(int type);
};

template <int T>
struct GeomProp; /*!< property of one xdmf type */

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
template <>
struct GeomProp<XdmfCellType::Polyvertex>
    : public BasicGeometry::Prop<BasicGeometry::POINT> {
  static const int numEdges = 0;
  static const int numFaces = 0;
};
template <>
struct GeomProp<XdmfCellType::Polyline>
    : public BasicGeometry::Prop<BasicGeometry::SEGMENT> {
  static const int maxEdgeVert = 2;
  static const int numFaces = 0;
};
template <>
struct GeomProp<XdmfCellType::Triangle>
    : public BasicGeometry::Prop<BasicGeometry::TRIANGLE> {
  static const int maxEdgeVert = 2;
};
template <>
struct GeomProp<XdmfCellType::Quadrilateral>
    : public BasicGeometry::Prop<BasicGeometry::SQUARE> {
  static const int maxEdgeVert = 2;
};
template <>
struct GeomProp<XdmfCellType::Tetrahedron>
    : public BasicGeometry::Prop<BasicGeometry::TETRAHEDRON> {
  static const int maxEdgeVert = 2;
  static const int faceTypes[4];
};
template <>
struct GeomProp<XdmfCellType::Hexahedron>
    : public BasicGeometry::Prop<BasicGeometry::CUBE> {
  static const int maxEdgeVert = 2;
  static const int faceTypes[6];
};
template <>
struct GeomProp<XdmfCellType::Wedge>
    : public BasicGeometry::Prop<BasicGeometry::PRISM> {
  static const int maxEdgeVert = 2;
  static const int faceTypes[5];
};
template <>
struct GeomProp<XdmfCellType::Pyramid>
    : public BasicGeometry::Prop<BasicGeometry::PYRAMID> {
  static const int maxEdgeVert = 2;
  static const int faceTypes[5];
};

/*! * * * * * * * * * * * * * * *  * * * * * * * * * * * * * */
template <>
struct GeomProp<XdmfCellType::Edge_3> {
  static const int dimension = 1;
  static const int numVert = 3;
  static const int numEdges = 1;
  static const int numFaces = 0;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numOrient = 2;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

template <>
struct GeomProp<XdmfCellType::Triangle_6> {
  static const int dimension = 2;
  static const int numVert = 6;
  static const int numEdges = 3;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 1;
  static const int faceVert[numFaces][numVert];
  static const int numOrient = 6;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

template <>
struct GeomProp<XdmfCellType::Quadrilateral_8> {
  static const int dimension = 2;
  static const int numVert = 8;
  static const int numEdges = 4;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 1;
  static const int faceVert[numFaces][numVert];
  static const int numOrient = 8;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

template <>
struct GeomProp<XdmfCellType::Quadrilateral_9> {
  static const int dimension = 2;
  static const int numVert = 9;
  static const int numEdges = 4;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 1;
  static const int faceVert[numFaces][numVert];
  static const int numOrient = 8;
  static const int orient[numOrient][numVert];
  static const int invOrient[numOrient];
};

template <>
struct GeomProp<XdmfCellType::Tetrahedron_10> {
  static const int dimension = 3;
  static const int numVert = 10;
  static const int numEdges = 6;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 4;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 6;
  static const int faceVert[numFaces][maxFaceVert];
};
template <>
struct GeomProp<XdmfCellType::Pyramid_13> {
  static const int dimension = 3;
  static const int numVert = 13;
  static const int numEdges = 8;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 5;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 8;
  static const int faceVert[numFaces][maxFaceVert];
};
template <>
struct GeomProp<XdmfCellType::Wedge_15> {
  static const int dimension = 3;
  static const int numVert = 15;
  static const int numEdges = 9;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 5;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 8;
  static const int faceVert[numFaces][maxFaceVert];
};
template <>
struct GeomProp<XdmfCellType::Wedge_18> {
  static const int dimension = 3;
  static const int numVert = 18;
  static const int numEdges = 9;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 5;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 9;
  static const int faceVert[numFaces][maxFaceVert];
};
template <>
struct GeomProp<XdmfCellType::Hexahedron_20> {
  static const int dimension = 3;
  static const int numVert = 20;
  static const int numEdges = 12;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 6;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 8;
  static const int faceVert[numFaces][maxFaceVert];
};
template <>
struct GeomProp<XdmfCellType::Hexahedron_27> {
  static const int dimension = 3;
  static const int numVert = 27;
  static const int numEdges = 12;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 6;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 9;
  static const int faceVert[numFaces][maxFaceVert];
};
template <>
struct GeomProp<XdmfCellType::Hexahedron_24> {
  static const int dimension = 3;
  static const int numVert = 24;
  static const int numEdges = 12;
  static const int maxEdgeVert = 3;
  static const int edges[numEdges][maxEdgeVert];
  static const int numFaces = 6;
  static const int faceTypes[numFaces];
  static const int maxFaceVert = 9;
  static const int faceVert[numFaces][maxFaceVert];
};

} /* End namespace HSF */

#endif /* end of include guard: MESH_GEOMETRY_HIGHORDERGEO_HPP */
