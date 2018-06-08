/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
namespace utils
{
	template< typename T >
	Mat4T< T >::Mat4T( NoInit const & n )noexcept
		: col0{ n }
		, col1{ n }
		, col2{ n }
		, col3{ n }
	{
	}

	template< typename T >
	Mat4T< T >::Mat4T()noexcept
		: Mat4T{ T{ 1 } }
	{
	}

	template< typename T >
	Mat4T< T >::Mat4T( std::array< T, 16u > const & v )noexcept
		: data{ v }
	{
	}

	template< typename T >
	template< typename U >
	Mat4T< T >::Mat4T( U const & v )noexcept
		: col0{ v, 0, 0, 0 }
		, col1{ 0, v, 0, 0 }
		, col2{ 0, 0, v, 0 }
		, col3{ 0, 0, 0, v }
	{
	}

	template< typename T >
	Mat4T< T >::Mat4T( Vec4T< T > const & col0
		, Vec4T< T > const & col1
		, Vec4T< T > const & col2
		, Vec4T< T > const & col3 )
		: col0{ col0 }
		, col1{ col1 }
		, col2{ col2 }
		, col3{ col3 }
	{
	}

	template< typename T >
	template< typename U >
	Mat4T< T >::Mat4T( Mat4T< U > const & rhs )noexcept
		: col0{ rhs.col0 }
		, col1{ rhs.col1 }
		, col2{ rhs.col2 }
		, col3{ rhs.col3 }
	{
	}

	template< typename T >
	template< typename U >
	Mat4T< T > & Mat4T< T >::operator=( Mat4T< U > const & rhs )noexcept
	{
		col0 = rhs.col0;
		col1 = rhs.col1;
		col2 = rhs.col2;
		col3 = rhs.col3;
		return *this;
	}

	template< typename T >
	Vec4T< T > & Mat4T< T >::operator[]( size_t index )noexcept
	{
		assert( index < 4 );
		return cols[index];
	}

	template< typename T >
	Vec4T< T > const & Mat4T< T >::operator[]( size_t index )const noexcept
	{
		assert( index < 4 );
		return cols[index];
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator+=( Mat4T< U > const & rhs )noexcept
	{
		col0 += rhs.col0;
		col1 += rhs.col1;
		col2 += rhs.col2;
		col3 += rhs.col3;
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator-=( Mat4T< U > const & rhs )noexcept
	{
		col0 -= rhs.col0;
		col1 -= rhs.col1;
		col2 -= rhs.col2;
		col3 -= rhs.col3;
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator*=( Mat4T< U > const & rhs )noexcept
	{
		Vec4T< T > const srcA0 = col0;
		Vec4T< T > const srcA1 = col1;
		Vec4T< T > const srcA2 = col2;
		Vec4T< T > const srcA3 = col3;

		Vec4T< T > const & srcB0 = rhs[0];
		Vec4T< T > const & srcB1 = rhs[1];
		Vec4T< T > const & srcB2 = rhs[2];
		Vec4T< T > const & srcB3 = rhs[3];

		col0 = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
		col1 = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
		col2 = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
		col3 = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator+=( U const & rhs )noexcept
	{
		col0 += rhs;
		col1 += rhs;
		col2 += rhs;
		col3 += rhs;
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator-=( U const & rhs )noexcept
	{
		col0 -= rhs;
		col1 -= rhs;
		col2 -= rhs;
		col3 -= rhs;
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator*=( U const & rhs )noexcept
	{
		col0 *= rhs;
		col1 *= rhs;
		col2 *= rhs;
		col3 *= rhs;
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator/=( U const & rhs )noexcept
	{
		col0 /= rhs;
		col1 /= rhs;
		col2 /= rhs;
		col3 /= rhs;
		return *this;
	}

	template< typename T >
	inline bool operator==( Mat4T< T > const & lhs
		, Mat4T< T > const & rhs )noexcept
	{
		return lhs[0] == rhs[0]
			&& lhs[1] == rhs[1]
			&& lhs[2] == rhs[2]
			&& lhs[3] == rhs[3];
	}

	template< typename T >
	inline bool operator!=( Mat4T< T > const & lhs
		, Mat4T< T > const & rhs )noexcept
	{
		return lhs[0] != rhs[0]
			|| lhs[1] != rhs[1]
			|| lhs[2] != rhs[2]
			|| lhs[3] != rhs[3];
	}

	template< typename T, typename U >
	inline Mat4T< T > operator+( Mat4T< T > const & lhs
		, Mat4T< U > const & rhs )noexcept
	{
		Mat4T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Mat4T< T > operator-( Mat4T< T > const & lhs
		, Mat4T< U > const & rhs )noexcept
	{
		Mat4T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Mat4T< T > operator*( Mat4T< T > const & lhs
		, Mat4T< U > const & rhs )noexcept
	{
		Mat4T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Mat4T< T > operator+( Mat4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Mat4T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Mat4T< T > operator-( Mat4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Mat4T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Mat4T< T > operator*( Mat4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Mat4T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Mat4T< T > operator/( Mat4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Mat4T< T > result{ lhs };
		result /= rhs;
		return result;
	}

	template< typename T >
	inline Vec4T< T > operator*( Mat4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept
	{
		Vec4T< T > const mov0( rhs.x, rhs.x, rhs.x, rhs.x );
		Vec4T< T > const mov1( rhs.y, rhs.y, rhs.y, rhs.y );
		Vec4T< T > const mul0 = lhs[0] * mov0;
		Vec4T< T > const mul1 = lhs[1] * mov1;
		Vec4T< T > const add0 = mul0 + mul1;
		Vec4T< T > const mov2( rhs.z, rhs.z, rhs.z, rhs.z );
		Vec4T< T > const mov3( rhs.w, rhs.w, rhs.w, rhs.w );
		Vec4T< T > const mul2 = lhs[2] * mov2;
		Vec4T< T > const mul3 = lhs[3] * mov3;
		Vec4T< T > const add1 = mul2 + mul3;
		Vec4T< T > const add2 = add0 + add1;
		return add2;
	}

	template< typename T >
	inline Mat4T< T > inverse( Mat4T< T > const & m )
	{
		T coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		T coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		T coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		T coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		T coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		T coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		T coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		T coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		T coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		T coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		T coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		T coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		T coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		T coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		T coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		T coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		T coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		T coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vec4T< T > fac0( coef00, coef00, coef02, coef03 );
		Vec4T< T > fac1( coef04, coef04, coef06, coef07 );
		Vec4T< T > fac2( coef08, coef08, coef10, coef11 );
		Vec4T< T > fac3( coef12, coef12, coef14, coef15 );
		Vec4T< T > fac4( coef16, coef16, coef18, coef19 );
		Vec4T< T > fac5( coef20, coef20, coef22, coef23 );

		Vec4T< T > vec0( m[1][0], m[0][0], m[0][0], m[0][0] );
		Vec4T< T > vec1( m[1][1], m[0][1], m[0][1], m[0][1] );
		Vec4T< T > vec2( m[1][2], m[0][2], m[0][2], m[0][2] );
		Vec4T< T > vec3( m[1][3], m[0][3], m[0][3], m[0][3] );

		Vec4T< T > inv0( vec1 * fac0 - vec2 * fac1 + vec3 * fac2 );
		Vec4T< T > inv1( vec0 * fac0 - vec2 * fac3 + vec3 * fac4 );
		Vec4T< T > inv2( vec0 * fac1 - vec1 * fac3 + vec3 * fac5 );
		Vec4T< T > inv3( vec0 * fac2 - vec1 * fac4 + vec2 * fac5 );

		Vec4T< T > signA( +1, -1, +1, -1 );
		Vec4T< T > signB( -1, +1, -1, +1 );
		Mat4T< T > inverted( inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB );

		Vec4T< T > row0( inverted[0][0], inverted[1][0], inverted[2][0], inverted[3][0] );

		Vec4T< T > dot0( m[0] * row0 );
		T dot1 = ( dot0.x + dot0.y ) + ( dot0.z + dot0.w );

		T oneOverDeterminant = static_cast< T >( 1 ) / dot1;

		return inverted * oneOverDeterminant;
	}
}
