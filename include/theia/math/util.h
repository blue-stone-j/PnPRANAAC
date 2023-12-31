// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_MATH_UTIL_H_
#define THEIA_MATH_UTIL_H_

#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

namespace theia {

static constexpr double kRadToDeg = 180.0 / M_PI;
static constexpr double kDegToRad = M_PI / 180.0;

inline double RadToDeg(double angle_radians) {
  return angle_radians * kRadToDeg;
}

inline double DegToRad(double angle_degrees) {
  return angle_degrees * kDegToRad;
}

inline double Clamp(const double val, const double min, const double max) {
  return std::max(min, std::min(val, max));
}

}  // namespace theia

#endif  // THEIA_MATH_UTIL_H_
