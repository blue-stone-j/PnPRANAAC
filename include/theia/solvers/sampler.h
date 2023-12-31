// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_SAMPLER_H_
#define THEIA_SOLVERS_SAMPLER_H_

#include <vector>

namespace theia {
// Purely virtual class used for the sampling consensus methods (e.g. Ransac,
// Prosac, MLESac, etc.)
template <class Datum> class Sampler {
 public:
  explicit Sampler(const int min_num_samples)
      : min_num_samples_(min_num_samples) {}

  // Initializes any non-trivial variables and sets up sampler if
  // necessary. Must be called before Sample is called.
  virtual bool Initialize() = 0;

  virtual ~Sampler() {}
  // Samples the input variable data and fills the vector subset with the
  // samples.
  virtual bool Sample(const std::vector<Datum>& data,
                      std::vector<Datum>* subset) = 0;

 protected:
  int min_num_samples_;
};

}  // namespace theia

#endif  // THEIA_SOLVERS_SAMPLER_H_
