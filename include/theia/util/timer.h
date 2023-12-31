// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_UTIL_TIMER_H_
#define THEIA_UTIL_TIMER_H_

#include <chrono>  // NOLINT

#include "theia/util/util.h"

namespace theia {

// Measures the elapsed wall clock time.
class Timer {
 public:
  // The constuctor starts a timer.
  Timer();

  // Resets the timer to zero.
  void Reset();

  // Measures the elapsed time since the last call to Reset() or the
  // construction of the object if Reset() has not been called. The resolution
  // of this timer is microseconds.
  double ElapsedTimeInSeconds();

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
  DISALLOW_COPY_AND_ASSIGN(Timer);
};

}  // namespace theia

#endif  // THEIA_UTIL_TIMER_H_
