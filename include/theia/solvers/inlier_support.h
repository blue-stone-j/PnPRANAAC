// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_INLIER_SUPPORT_H_
#define THEIA_SOLVERS_INLIER_SUPPORT_H_

#include <algorithm>
#include <vector>

#include "theia/solvers/quality_measurement.h"

namespace theia {
// Assess quality of data by whether each error residual is less than an error
// threshold. If it is below the threshold, it is considered an inlier.
class InlierSupport : public QualityMeasurement {
 public:
  explicit InlierSupport(const double error_thresh)
      : QualityMeasurement(error_thresh) {}
  ~InlierSupport() {}

  bool Initialize()  {
    max_inlier_ratio_ = 0.0;
    return true;
  }

  // Count the number of inliers in the data and return the cost such that lower
  // cost is better.
  double ComputeCost(const std::vector<double>& residuals) {
    double num_inliers = 0.0;
    for (int i = 0; i < residuals.size(); i++) {
      if (residuals[i] < this->error_thresh_) {
        num_inliers += 1.0;
      }
    }
    const double inlier_ratio =
        num_inliers / static_cast<double>(residuals.size());
    max_inlier_ratio_ = std::max(inlier_ratio, max_inlier_ratio_);
    return residuals.size() - num_inliers;
  }

  double GetInlierRatio() const { return max_inlier_ratio_; }

 private:
  double max_inlier_ratio_;
};

}  // namespace theia

#endif  // THEIA_SOLVERS_INLIER_SUPPORT_H_
