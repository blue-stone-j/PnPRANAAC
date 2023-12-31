// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_PROSAC_H_
#define THEIA_SOLVERS_PROSAC_H_

#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <vector>

#include "theia/solvers/estimator.h"
#include "theia/solvers/prosac_sampler.h"
#include "theia/solvers/sample_consensus_estimator.h"

namespace theia {
// Estimate a model using PROSAC. The Estimate method is inherited, but for
// PROSAC requires the data to be in sorted order by quality (with highest
// quality at index 0).
template <class ModelEstimator>
class Prosac : public SampleConsensusEstimator<ModelEstimator> {
 public:
  typedef typename ModelEstimator::Datum Datum;
  typedef typename ModelEstimator::Model Model;

  Prosac(const RansacParameters& ransac_params, const ModelEstimator& estimator)
      : SampleConsensusEstimator<ModelEstimator>(ransac_params, estimator) {}
  ~Prosac() {}

  bool Initialize() {
    Sampler<Datum>* prosac_sampler =
        new ProsacSampler<Datum>(this->estimator_.SampleSize());
    return SampleConsensusEstimator<ModelEstimator>::Initialize(prosac_sampler);
  }
};
}  // namespace theia

#endif  // THEIA_SOLVERS_PROSAC_H_
