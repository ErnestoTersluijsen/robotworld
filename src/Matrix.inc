#include <cassert>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <utility>
#include <iomanip>

/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( T value)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at( row).at( column) = value;
		}
	}
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( const std::initializer_list< T >& aList)
{
	// Check the arguments
	assert( aList.size() == M * N);

	auto row_iter = aList.begin();
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column, ++row_iter)
		{
			matrix.at( row).at( column) = *row_iter;
		}
	}
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( const std::initializer_list< std::initializer_list< T > >& aList)
{
	// Check the arguments, the static assert assures that there is at least 1 M and 1 N!
	assert( aList.size() == M && (*aList.begin()).size() == N);

	auto row_iter = aList.begin();
	for (std::size_t row = 0; row < aList.size(); ++row, ++row_iter)
	{
		auto column_iter = (*row_iter).begin();
		for (std::size_t column = 0; column < (*row_iter).size(); ++column, ++column_iter)
		{
			matrix.at( row).at( column) = *column_iter;
		}
	}
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >::Matrix( const Matrix< T, M, N >& aMatrix) :
				matrix( aMatrix.matrix)
{
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
std::array< T, N >& Matrix< T, M, N >::at( std::size_t aRowIndex)
{
	return matrix.at( aRowIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
const std::array< T, N >& Matrix< T, M, N >::at( std::size_t aRowIndex) const
{
	return matrix.at( aRowIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
T& Matrix< T, M, N >::at( 	std::size_t aRowIndex,
							std::size_t aColumnIndex)
{
	return matrix.at( aRowIndex).at( aColumnIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
const T& Matrix< T, M, N >::at( std::size_t aRowIndex,
								std::size_t aColumnIndex) const
{
	return matrix.at( aRowIndex).at( aColumnIndex);
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
std::array< T, N >& Matrix< T, M, N >::operator[]( std::size_t aRowIndex)
{
	return matrix[aRowIndex];
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
const std::array< T, N >& Matrix< T, M, N >::operator[]( std::size_t aRowIndex) const
{
	return matrix[aRowIndex];
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >& Matrix< T, M, N >::operator=( const Matrix< T, M, N >& rhs)
{
	if (this != &rhs)
	{
		matrix = rhs.matrix;
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
bool Matrix< T, M, N >::operator==( const Matrix< T, M, N >& rhs) const
{
	return matrix == rhs.matrix;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N >& Matrix< T, M, N >::operator*=( const T2& scalar)
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at( row).at( column) *= scalar;
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N > Matrix< T, M, N >::operator*( const T2& scalar) const
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	Matrix< T, M, N > result( *this);
	return result *= scalar;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N >& Matrix< T, M, N >::operator/=( const T2& aScalar)
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at( row).at( column) /= aScalar;
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
template< class T2 >
Matrix< T, M, N > Matrix< T, M, N >::operator/( const T2& aScalar) const
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	Matrix< T, M, N > result( *this);
	return result /= aScalar;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >& Matrix< T, M, N >::operator+=( const Matrix< T, M, N >& rhs)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix[row][column] += rhs.at( row, column);
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::operator+( const Matrix< T, M, N >& rhs) const
{
	Matrix< T, M, N > result( *this);
	return result += rhs;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N >& Matrix< T, M, N >::operator-=( const Matrix< T, M, N >& rhs)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix[row][column] -= rhs.at( row, column);
		}
	}
	return *this;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::operator-( const Matrix< T, M, N >& rhs) const
{
	Matrix< T, M, N > result( *this);
	return result -= rhs;
}
/**
 * (M, N) * (N, P) -> (M, P)
 */
template< typename T, std::size_t M, std::size_t N >
template< std::size_t columns >
Matrix< T, M, columns > Matrix< T, M, N >::operator*( const Matrix< T, N, columns >& rhs) const
{
	Matrix< T, M, columns > result;

	for (std::size_t result_row = 0; result_row < M; ++result_row)
	{
		for (std::size_t result_column = 0; result_column < columns; ++result_column)
		{
			T cell_result = 0;

			for(std::size_t cell_loop = 0; cell_loop < N; ++cell_loop)
			{
				cell_result += matrix[result_row][cell_loop] * rhs.at(cell_loop, result_column);
			}
			result.at(result_row, result_column) = cell_result;
		}
	}

	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, N, M > Matrix< T, M, N >::transpose() const
{
	Matrix< T, N, M > result;

	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			result.at(column, row) = matrix[row][column];
		}
	}

	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::identity() const
{
	Matrix< T, N, M > result;

	for (std::size_t row = 0; row < M; ++row) {
		for (std::size_t column = 0; column < N; ++column) {
			if (row == column)
			{
				result.at(row, column) = 1;
			}
			else
			{
				result.at(row, column) = 0;
			}
		}
	}
	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::gauss() const
{
	Matrix < T, M, N > result = *this;

	for (std::size_t iterator = 0; iterator < M; ++iterator)
	{
		std::size_t pivot_row_index = iterator;
		for (std::size_t j = 0; j < M; ++j)
		{
			if (std::abs(result.at(j, iterator)) > std::abs(result.at(pivot_row_index, iterator)))
			{
				pivot_row_index = j;
			}
		}

		if (pivot_row_index != iterator)
		{
			for (std::size_t col = 0; col < N; ++col)
			{
				std::swap(result.at(iterator, col), result.at(pivot_row_index, col));
			}
		}

		T pivot_value = result.at(iterator, iterator);
		for (std::size_t col = 0; col < N; ++col)
		{
			result.at(iterator, col) /= pivot_value;
		}

		for (std::size_t row = iterator + 1; row < M; ++row)
		{
			T factor = result.at(row, iterator);
			for (std::size_t col = iterator; col < N; ++col)
			{
				result.at(row, col) -= factor * result.at(iterator, col);
			}
		}
	}

	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::gaussJordan() const
{
	Matrix < T, M, N > result = *this;

	for (std::size_t iterator = 0; iterator < M; ++iterator)
	{
		std::size_t pivot_row_index = iterator;

		for (std::size_t row = iterator + 1; row < M; ++row) {
			if (std::abs(result.at(row, iterator)) > std::abs(result.at(pivot_row_index, iterator))) {
				pivot_row_index = row;
			}
		}

		if (pivot_row_index != iterator) {
			for (std::size_t col = 0; col < N; ++col) {
				std::swap(result.at(iterator, col), result.at(pivot_row_index, col));
			}
		}

		T pivot_value = result.at(iterator, iterator);
		for (std::size_t col = iterator; col < N; ++col) {
			result.at(iterator, col) /= pivot_value;
		}

		for (std::size_t row = 0; row < M; ++row) {
			if (row != iterator) {
				T factor = result.at(row, iterator);

				for (std::size_t col = iterator; col < N; ++col) {
					result.at(row, col) -= factor * result.at(iterator, col);
				}
			}
		}
	}
	return result;
}
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, 1 > Matrix< T, M, N >::solve() const
{
	Matrix < T, M, N > matrix = *this;

	matrix = matrix.gaussJordan();

	Matrix < T, M, 1 > result;
	for(size_t row = 0; row < M; ++row) {
		result.at(row, 0) = matrix.at(row , N - 1);
	}

	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
Matrix< T, M, N > Matrix< T, M, N >::inverse() const
{
	Matrix< T, M, N + N > working_matrix;
	Matrix< T, M, N > identity_matrix = identity();
	Matrix< T, M, N > result;

	for(std::size_t row = 0; row < M; ++row)
	{
		for(std::size_t column = 0; column < N; ++column)
		{
			working_matrix.at(row, column) = matrix[row][column];
			working_matrix.at(row, column + N) = identity_matrix.at(row, column);
		}
	}

	for (std::size_t iterator = 0; iterator < M; ++iterator)
	{
		std::size_t pivot_row_index = iterator;

		for (std::size_t row = iterator + 1; row < M; ++row)
		{
			if (std::abs(working_matrix.at(row, iterator)) > std::abs(working_matrix.at(pivot_row_index, iterator)))
			{
				pivot_row_index = row;
			}
		}

		if (pivot_row_index != iterator)
		{
			for (std::size_t col = 0; col < N + N; ++col)
			{
				std::swap(working_matrix.at(iterator, col), working_matrix.at(pivot_row_index, col));
			}
		}

		T pivot_value = working_matrix.at(iterator, iterator);
		for (std::size_t col = iterator; col < N + N; ++col)
		{
			working_matrix.at(iterator, col) /= pivot_value;
		}

		for (std::size_t row = 0; row < M; ++row)
		{
			if (row != iterator)
			{
				T factor = working_matrix.at(row, iterator);

				for (std::size_t col = iterator; col < N + N; ++col)
				{
					working_matrix.at(row, col) -= factor * working_matrix.at(iterator, col);
				}
			}
		}
	}


	for(std::size_t row = 0; row < M; ++row)
	{
		for(std::size_t column = 0; column < N; ++column)
		{
			result.at(row, column) = working_matrix.at(row, column + N);
		}
	}

	return result;
}
/**
 *
 */
template< class T, std::size_t M, std::size_t N >
std::string Matrix< T, M, N >::to_string() const
{
	std::string result = "Matrix<" + std::to_string( N) + "," + std::to_string( M) + ">\n{\n";
	for (std::size_t i = 0; i < M; ++i)
	{
		for (std::size_t j = 0; j < N; ++j)
		{
			result += std::to_string( matrix[i][j]) + ",";
		}
		result += "\n";
	}
	result += "}";
	return result;
}
/**
 *
 */
template< typename T, const std::size_t N >
bool equals(	const Matrix< T, 1, N >& lhs,
				const Matrix< T, 1, N >& rhs,
				const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
				const unsigned long aFactor /*= 1*/)
{
	for (std::size_t column = 0; column < N; ++column)
	{
		if(std::abs(lhs[0][column] - rhs[0][column]) > aPrecision * static_cast<double>(aFactor))
		{
			return false;
		}
	}
	return true;
}
/**
 *
 */
template< typename T, const std::size_t M >
bool equals(	const Matrix< T, M, 1 >& lhs,
				const Matrix< T, M, 1 >& rhs,
				const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
				const unsigned long aFactor /*= 1*/)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		if(std::abs(lhs[row][0] - rhs[row][0]) > aPrecision * static_cast<double>(aFactor))
		{
			return false;
		}
	}
	return true;
}
/**
 *
 */
template< typename T, const std::size_t M, const std::size_t N >
bool equals(	const Matrix< T, M, N >& lhs,
				const Matrix< T, M, N >& rhs,
				const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
				const unsigned long aFactor /*= 1*/)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			if(std::abs(lhs[row][column] - rhs[row][column]) > aPrecision * static_cast<double>(aFactor))
			{
				return false;
			}
		}
	}
	return true;
}


template<typename T, std::size_t Mstate, std::size_t Nstate, std::size_t Mtm, std::size_t Ntm>
void calculate(const Matrix<T, Mstate, Nstate> &m0, Matrix<T, Mtm, Ntm> &m1, std::size_t power)
{
    Matrix<T, Mtm, Ntm> temp = m1.identity();

    for(std::size_t i = 0; i < power; ++i)
    {
        temp = temp * m1;
    }

    Matrix<T, Mstate,  Nstate> endState = m0 * temp;
    std::cout  << endState << std::endl;
}


template<typename T, std::size_t Mstate, std::size_t Nstate, std::size_t Mtm, std::size_t Ntm>
Matrix<T, Mstate,  Nstate> endstate(const Matrix<T, Mstate, Nstate> &m0, Matrix<T, Mtm, Ntm> &m1, std::size_t power)
{
    Matrix<T, Mtm, Ntm> temp = m1.identity();

    for(std::size_t i = 0; i < power; ++i)
    {
        temp = temp * m1;
    }

    Matrix<T, Mstate,  Nstate> endState = m0 * temp;
    return endState;
}

template<typename T, std::size_t Mstate, std::size_t Nstate, std::size_t Mtm, std::size_t Ntm>
std::size_t steadyState(const Matrix<T, Mstate, Nstate> &m0, Matrix<T, Mtm, Ntm> &m1)
{
	std::size_t power = 0;
	while(!equals(endstate(m0, m1, power), endstate(m0, m1, power + 1), 0.01, 1))
	{
		power++;
	}

	return power;
}


template<typename T, std::size_t Mmu, std::size_t Nmu, std::size_t Msigma, std::size_t Nsigma, std::size_t Ma, std::size_t Na, std::size_t Mb, std::size_t Nb, std::size_t Mq, std::size_t Nq, std::size_t Mmeasurement, std::size_t Nmeasurement>
std::pair<Matrix<T, Mmu, Nmu>, Matrix<T, Msigma, Nsigma>> kalmanFilter(std::pair<Matrix<T, Mmu, Nmu>, Matrix<T, Msigma, Nsigma>> initial, Matrix< T, Ma, Na > A, Matrix< T, Mb, Nb > B, Matrix< T, Mq, Nq > Q, Matrix<T, Mmeasurement, Nmeasurement> measurement, T ontwikkeling)
{
	Matrix<T, Mmu, Nmu> predMu = predictStateVector(initial.first, A, B, ontwikkeling);

	Matrix<T, Msigma, Nsigma> predSigma = predictCovariance(initial.second, A);

	Matrix<T, Msigma , Nsigma> kalman = calculateKalman(predSigma, Q);

	Matrix<T, Mmeasurement , Nmeasurement> Z = calculateMeasurement(measurement, predSigma);

	Matrix<T, Mmu , Nmu> adjMu = calculateAdjustedStateVector(predMu, kalman, Z);

	Matrix<T, Msigma , Nsigma> adjSigma = calculateAdjustedCovarianceMatrix(kalman, predSigma);
	return {adjMu, adjSigma};
}



template< typename T, std::size_t Mmu, std::size_t Nmu, std::size_t Ma, std::size_t Na, std::size_t Mb, std::size_t Nb>
Matrix<T, Mmu, Nmu> predictStateVector(Matrix< T, Mmu, Nmu > mu, Matrix< T, Ma, Na > A, Matrix< T, Mb, Nb > B, T ontwikkeling)
{
	return A * mu + B * ontwikkeling;
}

template< typename T, std::size_t Msigma, std::size_t Nsigma, std::size_t Ma, std::size_t Na>
Matrix<T, Msigma, Nsigma> predictCovariance(Matrix< T, Msigma, Nsigma > sigma, Matrix< T, Ma, Na > A)
{
	Matrix<T, Msigma, Nsigma> predSigma = A * sigma * A.transpose();
	predSigma.at(0, 1) = 0;
	predSigma.at(1, 0) = 0;
	return predSigma;
}

template< typename T, std::size_t Msigma, std::size_t Nsigma, std::size_t Mq, std::size_t Nq>
Matrix<T, Msigma, Nsigma> calculateKalman(Matrix< T, Msigma, Nsigma > sigma, Matrix< T, Mq, Nq > Q)
{
	Matrix<T, Msigma, Nsigma> C = sigma.identity();
	return ((sigma * C.transpose()) * (C * sigma * C.transpose() + Q).inverse());
}

template< typename T, std::size_t Mmeasurement, std::size_t Nmeasurement, std::size_t Msigma, std::size_t Nsigma>
Matrix<T, Mmeasurement, Nmeasurement> calculateMeasurement(Matrix<T, Mmeasurement, Nmeasurement> measurement, Matrix< T, Msigma, Nsigma > sigma)
{
	return sigma.identity() * measurement;
}


template< typename T, std::size_t Mmu, std::size_t Nmu, std::size_t Mkalman, std::size_t Nkalman, std::size_t Mmeasurement, std::size_t Nmeasurement>
Matrix<T, Mmu, Nmu> calculateAdjustedStateVector(Matrix< T, Mmu, Nmu > mu, Matrix<T, Mkalman, Nkalman> kalman, Matrix<T, Mmeasurement, Nmeasurement> measurement)
{
	return mu + (kalman * (measurement - (kalman.identity() * mu)));
}

template< typename T, std::size_t Mkalman, std::size_t Nkalman, std::size_t Msigma, std::size_t Nsigma>
Matrix<T, Mkalman, Nkalman> calculateAdjustedCovarianceMatrix(Matrix< T, Mkalman, Nkalman > kalman, Matrix< T, Msigma, Nsigma > sigma)
{
	Matrix<T, Mkalman, Nkalman> C = sigma.identity();
	return (C - (kalman * C)) * sigma;
}




