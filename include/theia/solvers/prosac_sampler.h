// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SOLVERS_PROSAC_SAMPLER_H_
#define THEIA_SOLVERS_PROSAC_SAMPLER_H_

#include <glog/logging.h>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "theia/solvers/sampler.h"
#include "theia/util/random.h"

namespace theia {
// Prosac sampler used for PROSAC implemented according to "Matching with PROSAC
// - Progressive Sampling Consensus" by Chum and Matas.
template <class Datum> class ProsacSampler : public Sampler<Datum> {
 public:
  explicit ProsacSampler(const int min_num_samples)
      : Sampler<Datum>(min_num_samples) {}
  ~ProsacSampler() {}

  bool Initialize() {
    ransac_convergence_iterations_ = 20000;
    kth_sample_number_ = 1;
    InitRandomGenerator();
    return true;
  }

  // Set the sample such that you are sampling the kth prosac sample (Eq. 6).
  void SetSampleNumber(int k) { kth_sample_number_ = k; }

  // Samples the input variable data and fills the vector subset with the prosac
  // samples.
  // NOTE: This assumes that data is in sorted order by quality where data[i] is
  // of higher quality than data[j] for all i < j.
  bool Sample(const std::vector<Datum>& data, std::vector<Datum>* subset) {
    // Set t_n according to the PROSAC paper's recommendation.
    double t_n = ransac_convergence_iterations_;
    int n = this->min_num_samples_;
    // From Equations leading up to Eq 3 in Chum et al.
    for (int i = 0; i < this->min_num_samples_; i++) {
      t_n *= static_cast<double>(n - i) / (data.size() - i);
    }

    double t_n_prime = 1.0;
    // Choose min n such that T_n_prime >= t (Eq. 5).
    for (int t = 1; t <= kth_sample_number_; t++) {
      if (t > t_n_prime && n < data.size()) {
        double t_n_plus1 =
            (t_n * (n + 1.0)) / (n + 1.0 - this->min_num_samples_);
        t_n_prime += ceil(t_n_plus1 - t_n);
        t_n = t_n_plus1;
        n++;
      }
    }
    subset->reserve(this->min_num_samples_);
    if (t_n_prime < kth_sample_number_) {
      // Randomly sample m data points from the top n data points.
      std::vector<int> random_numbers;
      for (int i = 0; i < this->min_num_samples_; i++) {
        // Generate a random number that has not already been used.
        int rand_number;
        while (std::find(random_numbers.begin(), random_numbers.end(),
                         (rand_number = RandInt(0, n - 1))) !=
               random_numbers.end()) {
        }

        random_numbers.push_back(rand_number);

        // Push the *unique* random index back.
        subset->push_back(data[rand_number]);
      }
    } else {
      std::vector<int> random_numbers;
      // Randomly sample m-1 data points from the top n-1 data points.
      for (int i = 0; i < this->min_num_samples_ - 1; i++) {
        // Generate a random number that has not already been used.
        int rand_number;
        while (std::find(random_numbers.begin(), random_numbers.end(),
                         (rand_number = RandInt(0, n - 2))) !=
               random_numbers.end()) {
        }
        random_numbers.push_back(rand_number);

        // Push the *unique* random index back.
        subset->push_back(data[rand_number]);
      }
      // Make the last point from the nth position.
      subset->push_back(data[n]);
    }
    CHECK_EQ(subset->size(), this->min_num_samples_)
        << "Prosac subset is incorrect "
        << "size!";
    kth_sample_number_++;
    return true;
  }

 private:
  // Number of iterations of PROSAC before it just acts like ransac.
  int ransac_convergence_iterations_;

  // The kth sample of prosac sampling.
  int kth_sample_number_;
};

}  // namespace theia

#endif  // THEIA_SOLVERS_PROSAC_SAMPLER_H_
