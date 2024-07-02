#include "boundBox.hpp"

#define point HSF::Tensor1<scalar>

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline HSF::BoundBox::BoundBox()
    : min_(invertedBox.min()), max_(invertedBox.max()) {}

inline HSF::BoundBox::BoundBox(const point& pt) : min_(pt), max_(pt) {}

inline HSF::BoundBox::BoundBox(const point& min, const point& max)
    : min_(min), max_(max) {}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool HSF::BoundBox::empty() const {
  // Check each component for max < min
  for (direction dir = 0; dir < point::nComponents(); ++dir) {
    if (max_[dir] < min_[dir]) {
      return true;
    }
  }
  return false;
}

inline bool HSF::BoundBox::valid() const {
  // Check each component for max < min
  for (direction dir = 0; dir < point::nComponents(); ++dir) {
    if (max_[dir] < min_[dir]) {
      return false;
    }
  }

  return true;
}

inline const point& HSF::BoundBox::min() const { return min_; }

inline const point& HSF::BoundBox::max() const { return max_; }

inline point& HSF::BoundBox::min() { return min_; }

inline point& HSF::BoundBox::max() { return max_; }

inline point HSF::BoundBox::centre() const { return 0.5 * (min_ + max_); }

inline point HSF::BoundBox::midpoint() const { return this->centre(); }

inline point HSF::BoundBox::span() const { return (max_ - min_); }

inline scalar HSF::BoundBox::mag() const { return HSF::mag(max_ - min_); }

inline scalar HSF::BoundBox::volume() const { return cmptProduct(span()); }

inline scalar HSF::BoundBox::minDim() const { return cmptMin(span()); }

inline scalar HSF::BoundBox::maxDim() const { return cmptMax(span()); }

inline scalar HSF::BoundBox::avgDim() const { return cmptAvg(span()); }

inline label HSF::BoundBox::nDim() const {
  label ngood = 0;

  for (direction dir = 0; dir < point::nComponents(); ++dir) {
    const scalar diff = (max_[dir] - min_[dir]);
    if (diff < 0) {
      return -1;
    } else if (diff > 0) {
      ++ngood;
    }
  }

  return ngood;
}

inline void HSF::BoundBox::clear() {
  min_ = invertedBox.min();
  max_ = invertedBox.max();
}

inline void HSF::BoundBox::add(const BoundBox& bb) {
  min_ = ::HSF::min(min_, bb.min_);
  max_ = ::HSF::max(max_, bb.max_);
}

inline void HSF::BoundBox::add(const point& pt) {
  min_ = ::HSF::min(min_, pt);
  max_ = ::HSF::max(max_, pt);
}

inline void HSF::BoundBox::add(const std::vector<point>& points) {
  for (const point& p : points) {
    add(p);
  }
}
inline void HSF::BoundBox::add(label num, const point* points) {
  for (label i = 0; i < num; i++) add(points[i]);
}

inline bool HSF::BoundBox::overlaps(const BoundBox& bb) const {
  return (bb.max_.x() >= min_.x() && bb.min_.x() <= max_.x() &&
          bb.max_.y() >= min_.y() && bb.min_.y() <= max_.y() &&
          bb.max_.z() >= min_.z() && bb.min_.z() <= max_.z());
}

inline bool HSF::BoundBox::overlaps(const point& centre,
                                    const scalar radiusSqr) const {
  // Find out where centre is in relation to bb.
  // Find nearest point on bb.
  scalar distSqr = 0;

  for (direction dir = 0; dir < point::nComponents(); ++dir) {
    const scalar d0 = min_[dir] - centre[dir];
    const scalar d1 = max_[dir] - centre[dir];

    if ((d0 > 0) != (d1 > 0)) {
      // centre inside both extrema. This component does not add any
      // distance.
    } else if (std::fabs(d0) < std::fabs(d1)) {
      distSqr += d0 * d0;
    } else {
      distSqr += d1 * d1;
    }

    if (distSqr > radiusSqr) {
      return false;
    }
  }

  return true;
}

inline bool HSF::BoundBox::contains(const point& pt) const {
  return (min_.x() <= pt.x() && pt.x() <= max_.x() && min_.y() <= pt.y() &&
          pt.y() <= max_.y() && min_.z() <= pt.z() && pt.z() <= max_.z());
}

inline bool HSF::BoundBox::contains(const point& pt, scalar rtol) const {
  bool in = true;
  for (int i = 0; i < 3; i++) {
    scalar eps = rtol * (max_[i] - min_[i]);
    in &= pt[i] >= (min_[i] - eps);
    in &= pt[i] <= (max_[i] + eps);
  }

  return in;
}

inline bool HSF::BoundBox::contains(const BoundBox& bb) const {
  return contains(bb.min()) && contains(bb.max());
}

inline bool HSF::BoundBox::containsInside(const point& pt) const {
  return (min_.x() < pt.x() && pt.x() < max_.x() && min_.y() < pt.y() &&
          pt.y() < max_.y() && min_.z() < pt.z() && pt.z() < max_.z());
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void HSF::BoundBox::operator+=(const BoundBox& bb) { add(bb); }

// * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * * //

inline bool HSF::operator==(const BoundBox& a, const BoundBox& b) {
  return (a.min() == b.min()) && (a.max() == b.max());
}

inline bool HSF::operator!=(const BoundBox& a, const BoundBox& b) {
  return !(a == b);
}

#undef point
