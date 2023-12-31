/*
 * P3P_Kneip.cpp
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

#include "pnpsolvers/P3P_Kneip.h"
#include <Eigen/Geometry>

#include <vector>

using namespace std;
using namespace Eigen;

P3P_Kneip::P3P_Kneip()
{
}

P3P_Kneip::~P3P_Kneip()
{
}

int P3P_Kneip::computePoses(Matrix3d featureVectors, Matrix3d worldPoints, vector<Matrix<double, 3, 4>> &solutions) const
{
	// Extraction of world points
	Vector3d P1 = worldPoints.col(0);
	Vector3d P2 = worldPoints.col(1);
	Vector3d P3 = worldPoints.col(2);

	// Verification that world points are not colinear
	Vector3d temp1 = P2 - P1;
	Vector3d temp2 = P3 - P1;

	// better to use "std::abs((temp1.cross(temp2)).norm()) < 0.01"
	// or "std::abs((temp1.cross(temp2)).norm())/distance(p2,p3) < 0.01"
	if ((temp1.cross(temp2)).norm() == 0)
	{
		return -1;
	}

	// Extraction of feature vectors
	Vector3d f1 = featureVectors.col(0);
	Vector3d f2 = featureVectors.col(1);
	Vector3d f3 = featureVectors.col(2);

	// Creation of intermediate(中间的) camera frame
	Vector3d e1 = f1;
	Vector3d e3 = f1.cross(f2);
	e3 = e3 / e3.norm();
	Vector3d e2 = e3.cross(e1);

	Matrix3d T;
	T.col(0) = e1;				// ||f1||
	T.col(1) = e2;				// ||f1||
	T.col(2) = e3;				// ||1 ||
	T.transposeInPlace(); // transpose itself: for orthogonal matrix, tanspose = invert

	f3 = T * f3; // coordinate in new coordinate system

	// Reinforce that f3[2] > 0 for having theta in [0;pi]
	if (f3[2] > 0)
	{
		f1 = featureVectors.col(1);
		f2 = featureVectors.col(0);
		f3 = featureVectors.col(2);

		e1 = f1;
		e3 = f1.cross(f2);
		e3 = e3 / e3.norm();
		e2 = e3.cross(e1);

		T.col(0) = e1;
		T.col(1) = e2;
		T.col(2) = e3;
		T.transposeInPlace();

		f3 = T * f3;

		P1 = worldPoints.col(1);
		P2 = worldPoints.col(0);
		P3 = worldPoints.col(2);
	}

	// Creation of intermediate world frame: use P1 as origin
	Vector3d n1 = P2 - P1;
	n1 = n1 / n1.norm();
	Vector3d n3 = n1.cross(P3 - P1);
	n3 = n3 / n3.norm();
	Vector3d n2 = n3.cross(n1);

	Matrix3d N;
	N.col(0) = n1; //  1
	N.col(1) = n2; //  1
	N.col(2) = n3; //  1
	N.transposeInPlace();

	// Extraction of known parameters
	P3 = N * (P3 - P1); // coordinate in new coordinate system

	double d_12 = (P2 - P1).norm(); // |P1P2|
	double f_1 = f3[0] / f3[2];			// x/z
	double f_2 = f3[1] / f3[2];			// y/z
	double p_1 = P3[0];
	double p_2 = P3[1];

	// formular (3)
	double cos_beta = f1.dot(f2);
	double b = 1 / (1 - pow(cos_beta, 2)) - 1; // b is as defined in article; here is square
	// assign sign by cos according article
	if (cos_beta < 0)
	{
		b = -sqrt(b);
	}
	else
	{
		b = sqrt(b);
	}

	// Definition of temporary variables for avoiding multiple computation
	double f_1_pw2 = pow(f_1, 2); // x^2/z^2
	double f_2_pw2 = pow(f_2, 2); // y^2/z^2

	double p_1_pw2 = pow(p_1, 2);		// x^2
	double p_1_pw3 = p_1_pw2 * p_1; // x^3
	double p_1_pw4 = p_1_pw3 * p_1; // x^4

	double p_2_pw2 = pow(p_2, 2);		// y^2
	double p_2_pw3 = p_2_pw2 * p_2; // y^3
	double p_2_pw4 = p_2_pw3 * p_2; // y^4

	double d_12_pw2 = pow(d_12, 2); //|P1P2|^2
	double b_pw2 = pow(b, 2);				// 1/tan^4

	// Computation of factors of 4th degree polynomial; formular (11)
	Matrix<double, 5, 1> factors;
	// -y^2/z^2 * y^4 - y^4 * x^2/z^2 - y^4
	factors[0] = -f_2_pw2 * p_2_pw4 - p_2_pw4 * f_1_pw2 - p_2_pw4; // 0:a4, 1:a3
	factors[1] = 2 * p_2_pw3 * d_12 * b + 2 * f_2_pw2 * p_2_pw3 * d_12 * b - 2 * f_2 * p_2_pw3 * f_1 * d_12;
	factors[2] = -f_2_pw2 * p_2_pw2 * p_1_pw2 - f_2_pw2 * p_2_pw2 * d_12_pw2 * b_pw2 - f_2_pw2 * p_2_pw2 * d_12_pw2 + f_2_pw2 * p_2_pw4 + p_2_pw4 * f_1_pw2 + 2 * p_1 * p_2_pw2 * d_12 + 2 * f_1 * f_2 * p_1 * p_2_pw2 * d_12 * b - p_2_pw2 * p_1_pw2 * f_1_pw2 + 2 * p_1 * p_2_pw2 * f_2_pw2 * d_12 - p_2_pw2 * d_12_pw2 * b_pw2 - 2 * p_1_pw2 * p_2_pw2;
	factors[3] = 2 * p_1_pw2 * p_2 * d_12 * b + 2 * f_2 * p_2_pw3 * f_1 * d_12 - 2 * f_2_pw2 * p_2_pw3 * d_12 * b - 2 * p_1 * p_2 * d_12_pw2 * b;
	factors[4] = -2 * f_2 * p_2_pw2 * f_1 * p_1 * d_12 * b + f_2_pw2 * p_2_pw2 * d_12_pw2 + 2 * p_1_pw3 * d_12 - p_1_pw2 * d_12_pw2 + f_2_pw2 * p_2_pw2 * p_1_pw2 - p_1_pw4 - 2 * f_2_pw2 * p_2_pw2 * p_1 * d_12 + p_2_pw2 * f_1_pw2 * p_1_pw2 + f_2_pw2 * p_2_pw2 * d_12_pw2 * b_pw2;

	// Computation of roots
	Matrix<double, 4, 1> realRoots;
	this->solveQuartic(factors, realRoots);

	// Backsubstitution of each solution
	Matrix<double, 3, 4> gwc;
	for (int i = 0; i < 4; i++) // 4 solutions
	{
		double cot_alpha = (-f_1 * p_1 / f_2 - realRoots[i] * p_2 + d_12 * b) /
											 (-f_1 * realRoots[i] * p_2 / f_2 + p_1 - d_12); // formular (9)

		double cos_theta = realRoots[i];
		double sin_theta = sqrt(1 - pow(realRoots[i], 2));
		double sin_alpha = sqrt(1 / (pow(cot_alpha, 2) + 1));
		double cos_alpha = sqrt(1 - pow(sin_alpha, 2));

		if (cot_alpha < 0)
		{
			cos_alpha = -cos_alpha;
		}

		// formular (5)
		Vector3d C(d_12 * cos_alpha * (sin_alpha * b + cos_alpha),
							 cos_theta * d_12 * sin_alpha * (sin_alpha * b + cos_alpha),
							 sin_theta * d_12 * sin_alpha * (sin_alpha * b + cos_alpha));

		C = P1 + N.transpose() * C; // formular (12)

		Matrix3d R; // formular(6)
		R << -cos_alpha, -sin_alpha * cos_theta, -sin_alpha * sin_theta,
				sin_alpha, -cos_alpha * cos_theta, -cos_alpha * sin_theta,
				0, -sin_theta, cos_theta;

		R = N.transpose() * R.transpose() * T; // formular(13)
		gwc.block<3, 3>(0, 0) = R;
		gwc.block<3, 1>(0, 3) = C;
		solutions.emplace_back(gwc);
	} // endfor: have stored all 4 solutions

	return 0;
}

int P3P_Kneip::solveQuartic(Matrix<double, 5, 1> factors, Matrix<double, 4, 1> &realRoots) const
{
	double A = factors[0];
	double B = factors[1];
	double C = factors[2];
	double D = factors[3];
	double E = factors[4];

	double A_pw2 = A * A;
	double B_pw2 = B * B;
	double A_pw3 = A_pw2 * A;
	double B_pw3 = B_pw2 * B;
	double A_pw4 = A_pw3 * A;
	double B_pw4 = B_pw3 * B;

	double alpha = -3 * B_pw2 / (8 * A_pw2) + C / A;
	double beta = B_pw3 / (8 * A_pw3) - B * C / (2 * A_pw2) + D / A;
	double gamma = -3 * B_pw4 / (256 * A_pw4) + B_pw2 * C / (16 * A_pw3) - B * D / (4 * A_pw2) + E / A;

	double alpha_pw2 = alpha * alpha;
	double alpha_pw3 = alpha_pw2 * alpha;

	std::complex<double> P(-alpha_pw2 / 12 - gamma, 0);
	std::complex<double> Q(-alpha_pw3 / 108 + alpha * gamma / 3 - pow(beta, 2) / 8, 0);
	std::complex<double> R = -Q / 2.0 + sqrt(pow(Q, 2.0) / 4.0 + pow(P, 3.0) / 27.0);

	std::complex<double> U = pow(R, (1.0 / 3.0));
	std::complex<double> y;

	if (U.real() == 0)
	{
		y = -5.0 * alpha / 6.0 - pow(Q, (1.0 / 3.0));
	}
	else
	{
		y = -5.0 * alpha / 6.0 - P / (3.0 * U) + U;
	}

	std::complex<double> w = sqrt(alpha + 2.0 * y);

	std::complex<double> temp;

	temp = -B / (4.0 * A) + 0.5 * (w + sqrt(-(3.0 * alpha + 2.0 * y + 2.0 * beta / w)));
	realRoots[0] = temp.real();
	temp = -B / (4.0 * A) + 0.5 * (w - sqrt(-(3.0 * alpha + 2.0 * y + 2.0 * beta / w)));
	realRoots[1] = temp.real();
	temp = -B / (4.0 * A) + 0.5 * (-w + sqrt(-(3.0 * alpha + 2.0 * y - 2.0 * beta / w)));
	realRoots[2] = temp.real();
	temp = -B / (4.0 * A) + 0.5 * (-w - sqrt(-(3.0 * alpha + 2.0 * y - 2.0 * beta / w)));
	realRoots[3] = temp.real();

	return 0;
}
