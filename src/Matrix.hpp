#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <limits>

/**
 * The Matrix class is an implementation of the mathematical concept of a matrix (not the movie).
 * @see https://en.wikipedia.org/wiki/Matrix_(mathematics) for more information.
 *
 * typename T: T must be an arithmetic type, i.e. an integral or floating point type
 * const std::size_t M: rows, number of rows of the matrix
 * const std::size_t N: columns, number of columns of the matrix
 */
template< typename T, const std::size_t M /* number of rows */, const std::size_t N /* number of columns */>
class Matrix
{
	public:
		/**
		 * @name Compile-time assertion checking: see http://en.cppreference.com/w/cpp/language/static_assert
		 */
		//@{
		/**
		 *
		 */
		static_assert( std::is_arithmetic<T>::value, "Value T must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");
		/**
		 *
		 */
		static_assert( M > 0 && N > 0, "M (rows) and N (columns) must both be greater than 0");
		//@}
		/**
		 * @name Constructors and destructor
		 */
		//@{
		/**
		 * Default ctor. Initialises all cells with 0 or the given value.
		 */
		explicit Matrix( T value = 0);
		/**
		 * Ctor with a linear list of values that must contain M*N elements
		 */
		explicit Matrix( const std::initializer_list< T >& aList);
		/**
		 * Ctor with a list of lists of values where aList must contain M elements and each list in aList must contain N elements
		 */
		explicit Matrix( const std::initializer_list< std::initializer_list< T > >& aList);
		/**
		 * Cpy ctor
		 */
		Matrix( const Matrix< T, M, N >& aMatrix);
		/**
		 * Dtor
		 */
		virtual ~Matrix() = default;
		//@}
		/**
		 * @name Dimension access
		 */
		//@{
		/**
		 *
		 */
		inline static std::size_t getRows()
		{
			return M;
		}
		/**
		 *
		 */
		inline static std::size_t getColumns()
		{
			return N;
		}
		//@}
		/**
		 * @name Element access
		 */
		//@{
		/**
		 * Returns the row at aRowIndex
		 * If aRowIndex > getRows() an exception of type std::out_of_range is thrown.
		 */
		std::array< T, N >& at( std::size_t aRowIndex);
		/**
		 * Returns the row at aRowIndex
		 * If aRowIndex > getRows() an exception of type std::out_of_range is thrown.
		 */
		const std::array< T, N >& at( std::size_t aRowIndex) const;
		/**
		 * Returns the cell at (aRowIndex,aColumnIndex)
		 * If aRowIndex > getRows() or aColumnIndex > getColumns an exception of type std::out_of_range is thrown.
		 */
		T& at( 	std::size_t aRowIndex,
				std::size_t aColumnIndex);
		/**
		 * Returns the element at (aRowIndex,aColumnIndex)
		 * If aRowIndex > getRows() or aColumnIndex > getColumns an exception of type std::out_of_range is thrown.
		 */
		const T& at( 	std::size_t aRowIndex,
						std::size_t aColumnIndex) const;
		/**
		 * Returns the row at aRowIndex. No range checking is done.
		 */
		std::array< T, N >& operator[]( std::size_t aRowIndex);
		/**
		 * Returns the row at aRowIndex. No range checking is done.
		 */
		const std::array< T, N >& operator[]( std::size_t aRowIndex) const;
		//@}
		/**
		 * @name Matrix operators
		 */
		//@{
		/**
		 * Assignment operator
		 */
		Matrix< T, M, N >& operator=( const Matrix< T, M, N >& rhs);
		/**
		 * Comparison operator
		 */
		bool operator==( const Matrix< T, M, N>& rhs) const;
		//@}
		/**
		 * @name Scalar arithmetic operations supporting only rhs-scalars
		 */
		//@{
		/**
		 *
		 */
		template< class T2 = T >
		Matrix< T, M, N >& operator*=( const T2& scalar);
		/**
		 *
		 */
		template< class T2 = T >
		Matrix< T, M, N > operator*( const T2& scalar) const;
		/**
		 *
		 */
		template< class T2 = T >
		Matrix< T, M, N >& operator/=( const T2& scalar);
		/**
		 *
		 */
		template< class T2 = T >
		Matrix< T, M, N > operator/( const T2& scalar) const;
		//@}
		/**
		 * @name Matrix arithmetic operations
		 */
		//@{
		/**
		 *
		 */
		Matrix< T, M, N >& operator+=( const Matrix< T, M, N >& rhs);
		/**
		 *
		 */
		Matrix< T, M, N > operator+( const Matrix< T, M, N >& rhs) const;
		/**
		 *
		 */
		Matrix< T, M, N >& operator-=( const Matrix< T, M, N >& rhs);
		/**
		 *
		 */
		Matrix< T, M, N > operator-( const Matrix< T, M, N >& rhs) const;
		/**
		 * (M, N) * (N, O) -> (M, O)
		 */
		template< std::size_t columns>
		Matrix< T, M, columns>  operator*( const Matrix< T, N, columns >& rhs) const;
		//@}
		/**
		 * @name Matrix functions
		 */
		//@{
		/**
		 * @see https://en.wikipedia.org/wiki/Transpose
		 */
		Matrix< T, N, M > transpose() const;
		/**
		 * @see https://en.wikipedia.org/wiki/Identity_matrix
		 */
		Matrix< T, M, N > identity() const;
		/**
		 * @see https://en.wikipedia.org/wiki/Gaussian_elimination
		 */
		Matrix< T, M, N > gauss() const;
		/**
		 * @see https://en.wikipedia.org/wiki/Invertible_matrix
		 */
		Matrix< T, M, N > gaussJordan() const;
		/**
		 *
		 */
		Matrix< T, M, 1 > solve() const;
		/**
		 * @see https://en.wikipedia.org/wiki/Invertible_matrix
		 */
		Matrix< T, M, N > inverse() const;
		//@}
		/**
		 * @name Other methods
		 */
		//@{
		/**
		 * @return a string representation of the matrix for printing on the screen
		 */
		std::string to_string() const;
		//@}
	private:
		std::array< std::array< T, N >, M > matrix;
};

/**
 *
 */
template< typename T, std::size_t M, std::size_t N >
inline bool operator==( const Matrix< T, M, N >& lhs,
						const Matrix< T, M, N >& rhs)
{
	return lhs.matrix == rhs.matrix;
}
/**
 *
 */
template< typename T, std::size_t M, std::size_t N >
inline std::ostream& operator<<( std::ostream& stream,
								 const Matrix< T, M, N >& aMatrix)
{
	return stream << aMatrix.to_string();
}

/**
 * Compare two row vectors using a aPrecision and a factor. The actual used precision is aPrecission*aFactor.
 *
 */
template< typename T, const std::size_t N >
bool equals(const Matrix< T, 1, N>& lhs,
			const Matrix< T, 1, N>& rhs,
			const T aPrecision = std::numeric_limits<T>::epsilon(),
			const unsigned long aFactor = 1);
/**
 * Compare two column vectors using a aPrecision and a factor. The actual used precision is aPrecission*aFactor.
 */
template< typename T, const std::size_t M>
bool equals(const Matrix< T, M, 1>& lhs,
			const Matrix< T, M, 1>& rhs,
			const T aPrecision = std::numeric_limits<T>::epsilon(),
			const unsigned long aFactor = 1);
/**
 * Compare two matrices using a Precision and a factor. The actual used  precision is aPrecission*aFactor.
 * If all rows are equal according this aPrecission*aFactor true is returned, false otherwise
 */
template< typename T, const std::size_t M, const std::size_t N>
bool equals(const Matrix< T, M, N>& lhs,
			const Matrix< T, M, N>& rhs,
			const T aPrecision = std::numeric_limits<T>::epsilon(),
			const unsigned long aFactor = 1);

template<typename T, std::size_t Mstate, std::size_t Nstate, std::size_t Mtm, std::size_t Ntm>
void calculate(const Matrix<T, Mstate, Nstate> &m0, Matrix<T, Mtm, Ntm> &m1, std::size_t power);

template<typename T, std::size_t Mstate, std::size_t Nstate, std::size_t Mtm, std::size_t Ntm>
Matrix<T, Mstate,  Nstate> endstate(const Matrix<T, Mstate, Nstate> &m0, Matrix<T, Mtm, Ntm> &m1, std::size_t power);

template<typename T, std::size_t Mstate, std::size_t Nstate, std::size_t Mtm, std::size_t Ntm>
std::size_t steadyState(const Matrix<T, Mstate, Nstate> &m0, Matrix<T, Mtm, Ntm> &m1);





template<typename T, std::size_t Mmu, std::size_t Nmu, std::size_t Msigma, std::size_t Nsigma, std::size_t Ma, std::size_t Na, std::size_t Mb, std::size_t Nb, std::size_t Mq, std::size_t Nq, std::size_t Mmeasurement, std::size_t Nmeasurement>
std::pair<Matrix<T, Mmu, Nmu>, Matrix<T, Msigma, Nsigma>> kalmanFilter(std::pair<Matrix<T, Mmu, Nmu>, Matrix<T, Msigma, Nsigma>> initial, Matrix< T, Ma, Na > A, Matrix< T, Mb, Nb > B, Matrix< T, Mq, Nq > Q, Matrix<T, Mmeasurement, Nmeasurement> measurement, T ontwikkeling);

template< typename T, std::size_t Mmu, std::size_t Nmu, std::size_t Ma, std::size_t Na, std::size_t Mb, std::size_t Nb>
Matrix<T, Mmu, Nmu> predictStateVector(Matrix< T, Mmu, Nmu > mu, Matrix< T, Ma, Na > A, Matrix< T, Mb, Nb > B, T ontwikkeling);

template< typename T, std::size_t Msigma, std::size_t Nsigma, std::size_t Ma, std::size_t Na>
Matrix<T, Msigma, Nsigma> predictCovariance(Matrix< T, Msigma, Nsigma > sigma, Matrix< T, Ma, Na > A);

template< typename T, std::size_t Msigma, std::size_t Nsigma, std::size_t Mq, std::size_t Nq>
Matrix<T, Msigma, Nsigma> calculateKalman(Matrix< T, Msigma, Nsigma > sigma, Matrix< T, Mq, Nq > Q);

template< typename T, std::size_t Mmeasurement, std::size_t Nmeasurement, std::size_t Msigma, std::size_t Nsigma>
Matrix<T, Mmeasurement, Nmeasurement> calculateMeasurement(Matrix<T, Mmeasurement, Nmeasurement> measurement, Matrix< T, Msigma, Nsigma > sigma);

template< typename T, std::size_t Mmu, std::size_t Nmu, std::size_t Mkalman, std::size_t Nkalman, std::size_t Mmeasurement, std::size_t Nmeasurement>
Matrix<T, Mmu, Nmu> calculateAdjustedStateVector(Matrix< T, Mmu, Nmu > mu, Matrix<T, Mkalman, Nkalman> kalman, Matrix<T, Mmeasurement, Nmeasurement> measurement);

template< typename T, std::size_t Mkalman, std::size_t Nkalman, std::size_t Msigma, std::size_t Nsigma>
Matrix<T, Mkalman, Nkalman> calculateAdjustedCovarianceMatrix(Matrix< T, Mkalman, Nkalman > kalman, Matrix< T, Msigma, Nsigma > sigma);



#include "Matrix.inc"

#endif /* MATRIX_HPP_ */
