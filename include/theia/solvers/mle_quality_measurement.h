// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_MLE_QUALITY_MEASUREMENT_H_
#define THEIA_SOLVERS_MLE_QUALITY_MEASUREMENT_H_

#include <glog/logging.h>
#include <algorithm>
#include <limits>
#include <memory>
#include <vector>

#include "theia/math/distribution.h"
#include "theia/solvers/quality_measurement.h"

namespace theia {
// Define the quality metric according to Guided MLE from "MLESAC: A new robust
// estimator with application to estimating image geometry" by Torr.
class MLEQualityMeasurement : public QualityMeasurement {
 public:
  // Params:
  explicit MLEQualityMeasurement(const double error_thresh)
      : QualityMeasurement(error_thresh) {}

  ~MLEQualityMeasurement() {}

  bool Initialize()  {
    max_inlier_ratio_ = 0.0;
    return true;
  }

  // Given the residuals, assess a quality metric for the data. Returns the
  // quality assessment and outputs a vector of bools indicating the inliers.
  double ComputeCost(const std::vector<double>& residuals) {
    double num_inliers = 0.0;
    double mle_score = 0.0;
    for (int i = 0; i < residuals.size(); i++) {
      if (residuals[i] < error_thresh_) {
        num_inliers += 1.0;
        mle_score += residuals[i];
      } else {
        mle_score += error_thresh_;
      }
    }
    const double inlier_ratio =
        num_inliers / static_cast<double>(residuals.size());
    max_inlier_ratio_ = std::max(inlier_ratio, max_inlier_ratio_);
    return mle_score;
  }

  double GetInlierRatio() const { return max_inlier_ratio_; }

 private:
  double max_inlier_ratio_;
};

}  // namespace theia

#endif  // THEIA_SOLVERS_MLE_QUALITY_MEASUREMENT_H_
