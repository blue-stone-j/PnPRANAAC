/*
 * P3p.h
 *
 *  Created on: Nov 2, 2010
 *      Author: Laurent Kneip
 * Description: Compute the absolute pose of a camera using three 3D-to-2D correspondences
 *   Reference: A Novel Parametrization of the P3P-Problem for a Direct Computation of
 *              Absolute Camera Position and Orientation
 *
 *       Input: featureVectors: 3x3 matrix with UNITARY feature vectors (each column is a vector)
 *              worldPoints: 3x3 matrix with corresponding 3D world points (each column is a point)
 *              solutions: 3x16 matrix that will contain the solutions
 *                         form: [ 3x1 position(solution1) 3x3 orientation(solution1) 3x1 position(solution2) 3x3 orientation(solution2) ... ]
 *                         the obtained orientation matrices are defined as transforming points from the cam to the world frame
 *      Output: int: 0 if correct execution
 *                  -1 if world points aligned
 */

// article: <A Novel Parametrization of the Perspective-Three-Point Problem for
//           a Direct Computation of Absolute Camera Position and Orientation>
// You can find it in my repo "example" in folder "reference"

#pragma once
#include <Eigen/Core>
#include <Eigen/StdVector>

class P3P_Kneip
{
public:
	P3P_Kneip();
	virtual ~P3P_Kneip();

	int computePoses(Eigen::Matrix3d featureVectors, Eigen::Matrix3d worldPoints, std::vector<Eigen::Matrix<double, 3, 4>> &solutions) const;
	int solveQuartic(Eigen::Matrix<double, 5, 1> factors, Eigen::Matrix<double, 4, 1> &realRoots) const;
};
