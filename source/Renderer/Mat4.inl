/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
namespace renderer
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
	template< typename U >
	Mat4T< T >::Mat4T( U const & v )noexcept
		: col0{ v, 0, 0, 0 }
		, col1{ 0, v, 0, 0 }
		, col2{ 0, 0, v, 0 }
		, col3{ 0, 0, 0, v }
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
	}

	template< typename T >
	Vec4T< T > & Mat4T< T >::operator[]( size_t index )noexcept
	{
		assert( index < 4 );
		return data[index];
	}

	template< typename T >
	Vec4T< T > const & Mat4T< T >::operator[]( size_t index )const noexcept
	{
		assert( index < 4 );
		return data[index];
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator+=( Mat4T< U > const & rhs )noexcept
	{
		col0 += rhs.col0;
		col1 += rhs.col1;
		col2 += rhs.col2;
		col3 += rhs.col3;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator-=( Mat4T< U > const & rhs )noexcept
	{
		col0 -= rhs.col0;
		col1 -= rhs.col1;
		col2 -= rhs.col2;
		col3 -= rhs.col3;
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
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator-=( U const & rhs )noexcept
	{
		col0 -= rhs;
		col1 -= rhs;
		col2 -= rhs;
		col3 -= rhs;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator*=( U const & rhs )noexcept
	{
		col0 *= rhs;
		col1 *= rhs;
		col2 *= rhs;
		col3 *= rhs;
	}

	template< typename T >
	template< typename U >
	inline Mat4T< T > & Mat4T< T >::operator/=( U const & rhs )noexcept
	{
		col0 /= rhs;
		col1 /= rhs;
		col2 /= rhs;
		col3 /= rhs;
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
}
