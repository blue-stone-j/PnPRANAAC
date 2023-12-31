// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_MATH_DISTRIBUTION_H_
#define THEIA_MATH_DISTRIBUTION_H_

#include <glog/logging.h>
#include <stdio.h>
#include <cmath>

namespace theia {
// Abstract class for probability disributions.
class Distribution {
 public:
  Distribution() {}
  virtual ~Distribution() {}

  // Evaluate the disribution at x. NOTE: If eval results in a change of private
  // variables of a derived class, you should implement the changes in a
  // different method (i.e. an Update method)
  virtual double eval(double x) const = 0;
};

// Normal Gaussian Distribution.
class NormalDistribution : public Distribution {
 public:
  NormalDistribution(const double& mean, const double& sigma) {
    CHECK_GT(sigma, 0)
        << "Sigma must be greater than zero in a normal distribution";
    alpha_ = 1.0 / (sigma * sqrt(2.0 * M_PI));
    beta_ = -1.0 / (2.0 * sigma * sigma);
  }

  ~NormalDistribution() {}

  double eval(double x) const {
    return alpha_ * exp(beta_ * x * x); }

 private:
  // Normal factor.
  double alpha_;
  // Normal factor.
  double beta_;
};

// Uniform distribution between left and right. Probability is uniform when x is
// within this span, and 0 when x is outside of the span.
class UniformDistribution : public Distribution {
 public:
  UniformDistribution(const double& left, const double& right)
      : left_(left), right_(right) {
    CHECK_LT(left, right) << "Left bound must be less than the right bound for "
                          << "uniform distributions.";
    CHECK_NE(right, left) << "Left bound is equal to right bound! Uniform "
                          << "distribution must have a nonzero range.";
    inverse_span_ = (left == right) ? 1.0 : 1.0 / (right - left);
  }

  // Destructor
  ~UniformDistribution() {}

  double eval(double x) const {
    return (left_ <= x && x <= right_) ? inverse_span_ : 0;
  }

 protected:
  const double left_;
  const double right_;
  double inverse_span_;
};

}  // namespace theia

#endif  // THEIA_MATH_DISTRIBUTION_H_
