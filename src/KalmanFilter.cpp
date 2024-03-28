/*
 * KalmanFilter.cpp
 *
 *  Created on: Mar 21, 2024
 *      Author: ernesto
 */

#include <KalmanFilter.hpp>

#include <Matrix.hpp>

KalmanFilter::KalmanFilter() :
		stateVector( { { { 0 } }, { { 0 } } }),
		covarianceMatrix( { { 1, 0 }, { 0, 1 } })
{
}

void KalmanFilter::executeKalmanFilter(Matrix< double, 2, 2 > A, Matrix< double, 2, 2 > B, Matrix< double, 2, 2 > C, Matrix< double, 2, 1 > update, Matrix< double, 2, 2  > Q, Matrix< double, 2, 1 > measurement)
{
	stateVector = predictStateVector(stateVector, A, B, update);

	covarianceMatrix = predictCovarianceMatrix(covarianceMatrix, A);

	Matrix<double, 2, 2> kalmanGain = calculateKalmanGain(covarianceMatrix, C, Q);

	Matrix<double, 2, 1> measurementVector = calculateMeasurementVector(measurement, C);

	stateVector = calculateAdjustedStateVector(stateVector, kalmanGain, measurementVector, C);

	covarianceMatrix = calculateAdjustedCovarianceMatrix(C, kalmanGain, covarianceMatrix);
}


const Matrix<double, 2, 1>& KalmanFilter::getStateVector() const
{
	return stateVector;
}

void KalmanFilter::setStateVector(double x, double y)
{
	stateVector.at(0, 0) = x;
	stateVector.at(1, 0) = y;
}
