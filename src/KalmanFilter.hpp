/*
 * KalmanFilter.hpp
 *
 *  Created on: Mar 21, 2024
 *      Author: ernesto
 */

#ifndef SRC_KALMANFILTER_HPP_
#define SRC_KALMANFILTER_HPP_

#include "Matrix.hpp"

class KalmanFilter {
public:
	KalmanFilter();

private:
	Matrix<double, 2, 1> stateVector;
	Matrix<double, 2, 2> covarianceMatrix;
};

#endif /* SRC_KALMANFILTER_HPP_ */
