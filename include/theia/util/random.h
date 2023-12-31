// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_UTIL_RANDOM_H_
#define THEIA_UTIL_RANDOM_H_

namespace theia {
// Initializes the random generator to be based on the current time. Does not
// have to be called before calling RandDouble, but it works best if it is.
void InitRandomGenerator();

// Get a random double between lower and upper (inclusive).
double RandDouble(double lower, double upper);

// Get a random double between lower and upper (inclusive).
int RandInt(int lower, int upper);

// Generate a number drawn from a gaussian distribution.
double RandGaussian(double mean, double std_dev);

}  // namespace theia

#endif  // THEIA_UTIL_RANDOM_H_
