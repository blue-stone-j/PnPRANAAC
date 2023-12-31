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

#include "theia/math/find_polynomial_roots_companion_matrix.h"

#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include <glog/logging.h>

#include "theia/math/polynomial.h"
#include "theia/math/util.h"

namespace theia {
using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace {

// Balancing function as described by B. N. Parlett and C. Reinsch,
// "Balancing a Matrix for Calculation of Eigenvalues and Eigenvectors".
// In: Numerische Mathematik, Volume 13, Number 4 (1969), 293-304,
// Springer Berlin / Heidelberg. DOI: 10.1007/BF02165404
void BalanceCompanionMatrix(MatrixXd* companion_matrix_ptr) {
  CHECK_NOTNULL(companion_matrix_ptr);
  MatrixXd& companion_matrix = *companion_matrix_ptr;
  MatrixXd companion_matrix_offdiagonal = companion_matrix;
  companion_matrix_offdiagonal.diagonal().setZero();

  const int degree = companion_matrix.rows();

  // gamma <= 1 controls how much a change in the scaling has to
  // lower the 1-norm of the companion matrix to be accepted.
  //
  // gamma = 1 seems to lead to cycles (numerical issues?), so
  // we set it slightly lower.
  const double gamma = 0.9;

  // Greedily scale row/column pairs until there is no change.
  bool scaling_has_changed;
  do {
    scaling_has_changed = false;

    for (int i = 0; i < degree; ++i) {
      const double row_norm = companion_matrix_offdiagonal.row(i).lpNorm<1>();
      const double col_norm = companion_matrix_offdiagonal.col(i).lpNorm<1>();

      // Decompose row_norm/col_norm into mantissa * 2^exponent,
      // where 0.5 <= mantissa < 1. Discard mantissa (return value
      // of frexp), as only the exponent is needed.
      int exponent = 0;
      std::frexp(row_norm / col_norm, &exponent);
      exponent /= 2;

      if (exponent != 0) {
        const double scaled_col_norm = std::ldexp(col_norm, exponent);
        const double scaled_row_norm = std::ldexp(row_norm, -exponent);
        if (scaled_col_norm + scaled_row_norm < gamma * (col_norm + row_norm)) {
          // Accept the new scaling. (Multiplication by powers of 2 should not
          // introduce rounding errors (ignoring non-normalized numbers and
          // over- or underflow))
          scaling_has_changed = true;
          companion_matrix_offdiagonal.row(i) *= std::ldexp(1.0, -exponent);
          companion_matrix_offdiagonal.col(i) *= std::ldexp(1.0, exponent);
        }
      }
    }
  } while (scaling_has_changed);

  companion_matrix_offdiagonal.diagonal() = companion_matrix.diagonal();
  companion_matrix = companion_matrix_offdiagonal;
}

void BuildCompanionMatrix(const VectorXd& polynomial,
                          MatrixXd* companion_matrix_ptr) {
  CHECK_NOTNULL(companion_matrix_ptr);
  MatrixXd& companion_matrix = *companion_matrix_ptr;

  const int degree = polynomial.size() - 1;

  companion_matrix.resize(degree, degree);
  companion_matrix.setZero();
  companion_matrix.diagonal(-1).setOnes();
  companion_matrix.col(degree - 1) = -polynomial.reverse().head(degree);
}

}  // namespace

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
bool FindPolynomialRootsCompanionMatrix(const Eigen::VectorXd& polynomial_in,
                                        Eigen::VectorXd* real,
                                        Eigen::VectorXd* imaginary) {
  if (polynomial_in.size() == 0) {
    LOG(ERROR) << "Invalid polynomial of size 0 passed to FindPolynomialRoots";
    return false;
  }

  VectorXd polynomial = RemoveLeadingZeros(polynomial_in);
  const int degree = polynomial.size() - 1;

  // Is the polynomial constant?
  if (degree == 0) {
    LOG(WARNING) << "Trying to extract roots from a constant "
                 << "polynomial in FindPolynomialRootsCompanionMatrix";
    // We return true with no roots, not false, as if the polynomial is constant
    // it is correct that there are no roots. It is not the case that they were
    // there, but that we have failed to extract them.
    return true;
  }

  // Linear
  if (degree == 1) {
    FindLinearPolynomialRoots(polynomial, real, imaginary);
    return true;
  }

  // Quadratic
  if (degree == 2) {
    FindQuadraticPolynomialRoots(polynomial, real, imaginary);
    return true;
  }

  // The degree is now known to be at least 3. For cubic or higher
  // roots we use the method of companion matrices.

  // Divide by leading term
  const double leading_term = polynomial(0);
  polynomial /= leading_term;

  // Build and balance the companion matrix to the polynomial.
  MatrixXd companion_matrix(degree, degree);
  BuildCompanionMatrix(polynomial, &companion_matrix);
  BalanceCompanionMatrix(&companion_matrix);

  // Find its (complex) eigenvalues.
  Eigen::EigenSolver<MatrixXd> solver(companion_matrix, false);
  if (solver.info() != Eigen::Success) {
    LOG(ERROR) << "Failed to extract eigenvalues from companion matrix.";
    return false;
  }

  // Output roots
  if (real != NULL) {
    *real = solver.eigenvalues().real();
  } else {
    LOG(WARNING) << "NULL pointer passed as real argument to "
                 << "FindPolynomialRoots. Real parts of the roots will not "
                 << "be returned.";
  }
  if (imaginary != NULL) {
    *imaginary = solver.eigenvalues().imag();
  }
  return true;
}

}  // namespace theia
