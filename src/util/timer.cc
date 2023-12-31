// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#include "theia/util/timer.h"

#include <chrono> // NOLINT

namespace theia
{

  Timer::Timer()
  {
    start_ = std::chrono::high_resolution_clock::now();
  }

  void Timer::Reset()
  {
    start_ = std::chrono::high_resolution_clock::now();
  }

  double Timer::ElapsedTimeInSeconds()
  {
    const auto end = std::chrono::high_resolution_clock::now();
    return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count()) /
           (1000000.0);
  }

} // namespace theia
