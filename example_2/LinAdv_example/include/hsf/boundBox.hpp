#ifndef MESH_GEOMETRY_BOUNDBOX_HPP
#define MESH_GEOMETRY_BOUNDBOX_HPP
/*! \file BoundBox.hpp
 *  \author Zhao Chengpeng (chengpeng_zhao@foxmail.com)
 *  \date 2024-03-22
 *  \brief     A bounding box defined in terms of min/max extrema points.
 */

#include "basicGeo.hpp"
#include "tensor1.hpp"

#define point Tensor1<scalar>

namespace HSF {
class BoundBox {
  // Private data

  //- Minimum and maximum points describing the bounding box
  point min_, max_;

 public:
  //- A large BoundBox: min/max == -/+ ROOTVGREAT
  static const BoundBox greatBox;

  //- A large inverted BoundBox: min/max == +/- ROOTVGREAT
  static const BoundBox invertedBox;

  //- Faces to point addressing, as per a 'hex' cell
  static const int faces[6][4];

  //- The unit normal per face
  static const point faceNormals[6];

  // Constructors

  //- Construct without any points - an inverted bounding box
  inline BoundBox();

  //- Construct a bounding box containing a single initial point
  inline explicit BoundBox(const point& pt);

  //- Construct from components
  inline BoundBox(const point& min, const point& max);

  //- Construct as the bounding box of the given points
  //  Does parallel communication (doReduce = true)
  explicit BoundBox(label num, point* points, bool doReduce = true);

  //- Construct bounding box as an indirect subset of the points.
  //  The indices could be from cell/face etc.
  //  Does parallel communication (doReduce = true)
  // BoundBox(label num, point* points, const std::vector<label>& indices, bool
  // doReduce = true);

  // Member Functions

  // Access

  //- Bounding box is inverted, contains no points.
  inline bool empty() const;

  //- Bounding box is non-inverted.
  inline bool valid() const;

  //- Minimum describing the bounding box
  inline const point& min() const;

  //- Maximum describing the bounding box
  inline const point& max() const;

  //- Minimum describing the bounding box, non-const access
  inline point& min();

  //- Maximum describing the bounding box, non-const access
  inline point& max();

  //- The centre (midpoint) of the bounding box
  inline point centre() const;

  //- The midpoint (centre) of the bounding box. Identical to centre()
  inline point midpoint() const;

  //- The bounding box span (from minimum to maximum)
  inline point span() const;

  //- The magnitude of the bounding box span
  inline scalar mag() const;

  //- The volume of the bound box
  inline scalar volume() const;

  //- Smallest length/height/width dimension
  inline scalar minDim() const;

  //- Largest length/height/width dimension
  inline scalar maxDim() const;

  //- Average length/height/width dimension
  inline scalar avgDim() const;

  //- Count the number of positive, non-zero dimensions.
  //  \return -1 if any dimensions are negative,
  //  0 = 0D (point),
  //  1 = 1D (line aligned with an axis),
  //  2 = 2D (plane aligned with an axis),
  //  3 = 3D (box)
  inline label nDim() const;

  //- Corner points in an order corresponding to a 'hex' cell
  void getPoints(std::vector<point>& pts) const;

  //- Face midpoints
  void getFaceCentres(std::vector<point>& pts) const;

  //- Face centre of given face index
  point faceCentre(const direction facei) const;

  // Manipulate

  //- Clear bounding box and make it an inverted box
  inline void clear();

  //- Extend to include the second box.
  inline void add(const BoundBox& bb);

  //- Extend to include the point.
  inline void add(const point& pt);

  //- Extend to include the points.
  inline void add(const std::vector<point>& points);
  inline void add(label num, const point* points);

  //- Inflate box by factor*mag(span) in all dimensions
  void inflate(const scalar s);

  //- Parallel reduction of min/max values
  void reduce();

  // Query

  //- Intersection (union) with the second box.
  //  The return value is true if the intersection is non-empty.
  bool intersect(const BoundBox& bb);

  //- Overlaps/touches boundingBox?
  inline bool overlaps(const BoundBox& bb) const;

  //- Overlaps boundingSphere (centre + sqr(radius))?
  inline bool overlaps(const point& centre, const scalar radiusSqr) const;

  //- Contains point? (inside or on edge)
  inline bool contains(const point& pt) const;

  //- Contains point? (inside or on edge)
  inline bool contains(const point& pt, scalar eps) const;

  //- Fully contains other boundingBox?
  inline bool contains(const BoundBox& bb) const;

  //- Contains point? (inside only)
  inline bool containsInside(const point& pt) const;

  //- Contains all points? (inside or on edge)
  bool contains(const std::vector<point>& points) const;

  //- Contains any of the points? (inside or on edge)
  bool containsAny(const std::vector<point>& points) const;

  //- Return the nearest point on the BoundBox to the supplied point.
  //  If point is inside the BoundBox then the point is returned
  //  unchanged.
  point nearest(const point& pt) const;

  // Member Operators

  //- Extend box to include the second box, as per the add() method.
  inline void operator+=(const BoundBox& bb);

  // print boxes to vtk;
  static void printBoxes(std::string fileName, int dim, label num,
                         BoundBox* boxes);
};

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==(const BoundBox& a, const BoundBox& b);
inline bool operator!=(const BoundBox& a, const BoundBox& b);

} /* End namespace HSF */

#undef point
#include "boundBox.H"

#endif /* end of include guard: MESH_GEOMETRY_BOUNDBOX_HPP */
