// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_MATH_PROBABILITY_SEQUENTIAL_PROBABILITY_RATIO_H_
#define THEIA_MATH_PROBABILITY_SEQUENTIAL_PROBABILITY_RATIO_H_

#include <vector>

namespace theia {
// Modified version of Wald's SPRT as Matas et. al. implement it in "Randomized
// RANSAC with Sequential Probability Ratio Test"

// Calculates the decision threshold (A) based on the input parameters.
// sigma: Probability of rejecting a good model (Bernoulli parameter).
// epsilon: Inlier ratio.
// time_compute_model_ratio: Computing the model parameters from a sample takes
//   the same time as verification of time_compute_model_ratio data points.
//   Matas et. al. use 200.
// num_model_verified: Number of models that are verified per sample.
double CalculateSPRTDecisionThreshold(double sigma, double epsilon,
                                      double time_compute_model_ratio = 200.0,
                                      int num_models_verified = 1);

// Modified version of Wald's SPRT as Matas et. al. implement it in "Randomized
// RANSAC with Sequential Probability Ratio Test". See the paper for more
// details.
// residuals: error residuals to use for SPRT analysis.
// error_thresh: Error threshold for determining when Datum fits the model.
// sigma: Probabiliyt of rejecting a good model.
// epsilon: Inlier ratio.
// decision_threshold: The decision threshold at which to terminate.
// observed_inlier_ratio: Output parameter of inlier ratio tested.
bool SequentialProbabilityRatioTest(const std::vector<double>& residuals,
                                    double error_thresh, double sigma,
                                    double epsilon, double decision_threshold,
                                    int* num_tested_points,
                                    double* observed_inlier_ratio);

}  // namespace theia

#endif  // THEIA_MATH_PROBABILITY_SEQUENTIAL_PROBABILITY_RATIO_H_
