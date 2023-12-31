// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_QUALITY_MEASUREMENT_H_
#define THEIA_SOLVERS_QUALITY_MEASUREMENT_H_

#include <vector>

namespace theia {
// Purely virtual class to be used with sampling consensus estimators
// (e.g. Ransac, Prosac, MLESac, etc.). This class is implemented to assess the
// quality of the data. A trivial example is the inlier quality measurement
// (i.e. if the error for a measurement is less than a threshold, then it is an
// inlier).
class QualityMeasurement {
 public:
  explicit QualityMeasurement(const double error_thresh)
      : error_thresh_(error_thresh) {}
  virtual ~QualityMeasurement() {}

  // Initializes any non-trivial variables and sets up sampler if
  // necessary. Must be called before Compare is called.
  virtual bool Initialize() { return true; }

  // Given the residuals, assess a quality metric for the data. This is a cost,
  // so lower is better.
  virtual double ComputeCost(const std::vector<double>& residuals) = 0;

  // Returns the maximum inlier ratio found thus far through the Compare
  // call. This is used in SampleConsensusEstimator to recompute the necessary
  // number of iterations.
  virtual double GetInlierRatio() const = 0;

 protected:
  double error_thresh_;
};

}  // namespace theia

#endif  // THEIA_SOLVERS_QUALITY_MEASUREMENT_H_
