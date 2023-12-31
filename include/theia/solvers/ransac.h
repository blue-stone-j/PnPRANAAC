// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_RANSAC_H_
#define THEIA_SOLVERS_RANSAC_H_

#include <math.h>
#include <cstdlib>
#include <vector>

#include "theia/solvers/estimator.h"
#include "theia/solvers/random_sampler.h"
#include "theia/solvers/sample_consensus_estimator.h"

namespace theia {
template <class ModelEstimator>
class Ransac : public SampleConsensusEstimator<ModelEstimator> {
 public:
  typedef typename ModelEstimator::Datum Datum;
  typedef typename ModelEstimator::Model Model;

  explicit Ransac(const RansacParameters& ransac_params,
                  const ModelEstimator& estimator)
      : SampleConsensusEstimator<ModelEstimator>(ransac_params, estimator) {}
  virtual ~Ransac() {}

  // Initializes the random sampler and inlier support measurement.
  bool Initialize() {
    Sampler<Datum>* random_sampler =
        new RandomSampler<Datum>(this->estimator_.SampleSize());
    return SampleConsensusEstimator<ModelEstimator>::Initialize(random_sampler);
  }
};

}  // namespace theia

#endif  // THEIA_SOLVERS_RANSAC_H_
