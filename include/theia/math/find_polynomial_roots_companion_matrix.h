// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

// These functions are largely based off the the Ceres solver polynomial
// functions which are not available through the public interface. The license
// is below:
//
// Ceres Solver - A fast non-linear least squares minimizer
// Copyright 2012 Google Inc. All rights reserved.
// http://code.google.com/p/ceres-solver/
//
// Author: moll.markus@arcor.de (Markus Moll)
//         sameeragarwal@google.com (Sameer Agarwal)

#ifndef THEIA_MATH_FIND_POLYNOMIAL_ROOTS_COMPANION_MATRIX_H_
#define THEIA_MATH_FIND_POLYNOMIAL_ROOTS_COMPANION_MATRIX_H_

#include <Eigen/Core>

namespace theia {

// All polynomials are assumed to be the form
//
//   sum_{i=0}^N polynomial(i) x^{N-i}.
//
// and are given by a vector of coefficients of size N + 1.

// Use the companion matrix eigenvalues to determine the roots of the
// polynomial.
//
// This function returns true on success, false otherwise.
// Failure indicates that the polynomial is invalid (of size 0) or
// that the eigenvalues of the companion matrix could not be computed.
// On failure, a more detailed message will be written to LOG(ERROR).
// If real is not NULL, the real parts of the roots will be returned in it.
// Likewise, if imaginary is not NULL, imaginary parts will be returned in it.
bool FindPolynomialRootsCompanionMatrix(const Eigen::VectorXd& polynomial,
                                        Eigen::VectorXd* real,
                                        Eigen::VectorXd* imaginary);

}  // namespace theia

#endif  // THEIA_MATH_FIND_POLYNOMIAL_ROOTS_COMPANION_MATRIX_H_
