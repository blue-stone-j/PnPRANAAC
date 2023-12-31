// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#include "theia/util/random.h"

#include <glog/logging.h>
#include <chrono>
#include <random>

namespace theia
{
  namespace
  {
    std::default_random_engine util_generator;
  } // namespace

  // Initializes the random generator to be based on the current time. Does not
  // have to be called before calling RandDouble, but it works best if it is.
  void InitRandomGenerator()
  {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    util_generator.seed(seed);
  }

  // Get a random double between lower and upper (inclusive).
  double RandDouble(double lower, double upper)
  {
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(util_generator);
  }

  // Get a random int between lower and upper (inclusive).
  int RandInt(int lower, int upper)
  {
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(util_generator);
  }

  // Gaussian Distribution with the corresponding mean and std dev.
  double RandGaussian(double mean, double std_dev)
  {
    std::normal_distribution<double> distribution(mean, std_dev);
    return distribution(util_generator);
  }

} // namespace theia
