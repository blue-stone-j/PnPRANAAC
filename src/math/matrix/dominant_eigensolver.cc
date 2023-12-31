// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#include "theia/math/matrix/dominant_eigensolver.h"

#include <Eigen/Core>
#include <glog/logging.h>
#include "theia/math/matrix/linear_operator.h"

namespace theia
{

  bool DominantEigensolver::Compute(double *eigenvalue,
                                    Eigen::VectorXd *eigenvector) const
  {
    Eigen::VectorXd previous_eigenvector(A_.Cols());
    previous_eigenvector.setRandom();
    for (int i = 0; i < options_.max_num_iterations; i++)
    {
      *eigenvector = previous_eigenvector.normalized();
      A_.RightMultiply(*eigenvector, &previous_eigenvector);
      *eigenvalue = eigenvector->dot(previous_eigenvector);

      const double error =
          (*eigenvector * (*eigenvalue) - previous_eigenvector).stableNorm();
      VLOG(3) << "Iteration: " << i << "\tCurrent error = " << error;
      if (error < std::abs(*eigenvalue) * options_.tolerance)
      {
        VLOG(2) << "Power iterations converged after " << i + 1 << " iterations.";
        return true;
      }
    }

    return false;
  }

} // namespace theia
