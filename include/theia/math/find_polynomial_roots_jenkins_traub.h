// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_MATH_FIND_POLYNOMIAL_ROOTS_JENKINS_TRAUB_H_
#define THEIA_MATH_FIND_POLYNOMIAL_ROOTS_JENKINS_TRAUB_H_

#include <Eigen/Core>

namespace theia {

// A three-stage algorithm for finding roots of polynomials with real
// coefficients as outlined in: "A Three-Stage Algorithm for Real Polynomaials
// Using Quadratic Iteration" by Jenkins and Traub, SIAM 1970. Please note that
// this variant is different than the complex-coefficient version, and is
// estimated to be up to 4 times faster.
//
// The algorithm works by computing shifts in so-called "K-polynomials" that
// deflate the polynomial to reveal the roots. Once a root is found (or in the
// real-polynomial case, a pair of roots) then it is divided from the polynomial
// and the process is repeated.
bool FindPolynomialRootsJenkinsTraub(const Eigen::VectorXd& polynomial,
                                     Eigen::VectorXd* real_roots,
                                     Eigen::VectorXd* complex_roots);

}  // namespace theia

#endif  // THEIA_MATH_FIND_POLYNOMIAL_ROOTS_JENKINS_TRAUB_H_
