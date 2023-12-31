// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_MATH_HISTOGRAM_H_
#define THEIA_MATH_HISTOGRAM_H_

#include <string>
#include <vector>

#include "theia/util/stringprintf.h"

namespace theia {

// A simple histogram counter that will create a histogram composed of fixed
// bins that are specified by the user.
template <typename T>
class Histogram {
 public:
  // Initialize the historgram with its bins. The boundaries vector must be
  // sorted.
  explicit Histogram(const std::vector<T>& boundaries)
      : boundaries_(boundaries) {
    // Insert the data type's min and max to the front and back respectively.
    boundaries_.insert(boundaries_.begin(),
                       -std::numeric_limits<T>::infinity());
    boundaries_.push_back(std::numeric_limits<T>::max());
    histogram_count_.resize(boundaries_.size());
  }

  // Add a value to the histogram. The value will be added to the appropriate
  // histogram bin.
  void Add(const T value) {
    // Find the first boundary element that is greater than value.
    const auto& it =
        std::upper_bound(boundaries_.begin(), boundaries_.end(), value);
    const int bin_index = std::distance(boundaries_.begin(), it) - 1;
    ++histogram_count_[bin_index];
  }

  // Returns the histogram printed in a message. For example, a histogram with
  // boundaries of [0, 1, 2, 3] may return a printed string of:
  //    < 0 = 2
  //    [0 - 1) = 5
  //    [1 - 2) = 3
  //    [2 - 3) = 7
  //    > 3 = 2
  std::string PrintString() {
    std::string msg = "";

    // Print the bin containing elements less then the lower bound. Only print
    // this range if the bin is not empty.
    if (histogram_count_.front() > 0) {
      msg += theia::StringPrintf("< %s = %d",
                                 std::to_string(boundaries_[1]).c_str(),
                                 histogram_count_.front());
      msg += "\n";
    }

    for (int i = 1; i < boundaries_.size() - 2; i++) {
      msg += theia::StringPrintf("[%s - %s) = %d ",
                                 std::to_string(boundaries_[i]).c_str(),
                                 std::to_string(boundaries_[i + 1]).c_str(),
                                 histogram_count_[i]);
      msg += "\n";
    }

    // Print the bin containing elements greater then the input upper
    // bound. Only print this range if the bin is not empty.
    const int max_boundary_index = boundaries_.size() - 2;
    if (histogram_count_[max_boundary_index] > 0) {
      msg += theia::StringPrintf(
          "> %s = %d",
          std::to_string(boundaries_[max_boundary_index]).c_str(),
          histogram_count_[max_boundary_index]);
    }
    return msg;
  }

 private:
  std::vector<T> boundaries_;
  std::vector<int> histogram_count_;
  int below_minimum_boundary_count_;
};

}  // namespace theia

#endif  // THEIA_MATH_HISTOGRAM_H_
