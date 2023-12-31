// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_MATH_MATRIX_DOMINANT_EIGENSOLVER_H_
#define THEIA_MATH_MATRIX_DOMINANT_EIGENSOLVER_H_

#include <Eigen/Core>

#include "theia/math/matrix/linear_operator.h"

namespace theia
{

  // Computes the dominant eigenvalue of the matrix specified by the linear
  // operator A by using the Power method. This method can also be used to compute
  // the smallest eigenvalue or the eigenvalue nearest to a value b by
  // implementing (A - b * I)^-1 as the RightMultiply of the linear operator. This
  // can be done efficiently with a linear solve.
  class DominantEigensolver
  {
  public:
    struct Options
    {
      // Maximum number of iterations to run.
      int max_num_iterations = 100;

      // The tolerance for determining when a solution has converged.
      double tolerance = 1e-6;
    };

    DominantEigensolver(const Options &options, const LinearOperator &A)
        : options_(options), A_(A) {}

    // Computes the dominant eigenvalue and eigenvector using the Power iteration
    // method.
    bool Compute(double *eigenvalue, Eigen::VectorXd *eigenvector) const;

  private:
    const Options options_;
    const LinearOperator &A_;
  };

} // namespace theia

#endif // THEIA_MATH_MATRIX_DOMINANT_EIGENSOLVER_H_
