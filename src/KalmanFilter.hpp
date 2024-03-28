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

	void executeKalmanFilter(Matrix< double, 2, 2 > A, Matrix< double, 2, 2 > B, Matrix< double, 2, 2 > C, Matrix< double, 2, 1 > update, Matrix< double, 2, 2  > Q, Matrix< double, 2, 1 > measurement);

	const Matrix<double, 2, 1>& getStateVector() const;

	void setStateVector(double x, double y);
	
private:
	Matrix<double, 2, 1> stateVector;
	Matrix<double, 2, 2> covarianceMatrix;
};

#endif /* SRC_KALMANFILTER_HPP_ */
