// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_RANDOM_SAMPLER_H_
#define THEIA_SOLVERS_RANDOM_SAMPLER_H_

#include <stdlib.h>
#include <algorithm>
#include <vector>

#include "theia/solvers/sampler.h"
#include "theia/util/random.h"

namespace theia {

// Random sampler used for RANSAC. This is guaranteed to generate a unique
// sample by performing a Fisher-Yates sampling.
template <class Datum> class RandomSampler : public Sampler<Datum> {
 public:
  explicit RandomSampler(const int min_num_samples)
      : Sampler<Datum>(min_num_samples) {}
  ~RandomSampler() {}

  bool Initialize() {
    InitRandomGenerator();
    return true;
  }

  // Samples the input variable data and fills the vector subset with the
  // random samples.
  bool Sample(const std::vector<Datum>& data, std::vector<Datum>* subset) {
    subset->resize(this->min_num_samples_);
    std::vector<int> random_numbers(data.size());
    for (int i = 0; i < data.size(); i++) {
      random_numbers[i] = i;
    }

    for (int i = 0; i < this->min_num_samples_; i++) {
      std::swap(random_numbers[i], random_numbers[RandInt(i, data.size() - 1)]);
      (*subset)[i] = data[random_numbers[i]];
    }

    return true;
  }
};

}  // namespace theia

#endif  // THEIA_SOLVERS_RANDOM_SAMPLER_H_
