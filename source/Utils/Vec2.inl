/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#include <cassert>

namespace utils
{
	template< typename T >
	Vec2T< T >::Vec2T( NoInit const & )noexcept
	{
	}

	template< typename T >
	constexpr Vec2T< T >::Vec2T()noexcept
		: x{}
		, y{}
	{
	}

	template< typename T >
	template< typename X
		, typename Y>
	constexpr Vec2T< T >::Vec2T( X const & x
		, Y const & y )noexcept
		: x{ T( x ) }
		, y{ T( y ) }
	{
	}

	template< typename T >
	template< typename U >
	Vec2T< T >::Vec2T( Vec2T< U > const & rhs )noexcept
		: x{ T( rhs.x ) }
		, y{ T( rhs.y ) }
	{
	}

	template< typename T >
	constexpr Vec2T< T >::Vec2T( T const & v )noexcept
		: x{ v }
		, y{ v }
	{
	}

	template< typename T >
	template< typename U >
	Vec2T< T > & Vec2T< T >::operator=( Vec2T< U > const & rhs )noexcept
	{
		x = T( rhs.x );
		y = T( rhs.y );
	}

	template< typename T >
	T & Vec2T< T >::operator[]( size_t index )noexcept
	{
		assert( index < 2 );
		return data[index];
	}

	template< typename T >
	T const & Vec2T< T >::operator[]( size_t index )const noexcept
	{
		assert( index < 2 );
		return data[index];
	}

	template< typename T >
	template< typename U >
	inline Vec2T< T > & Vec2T< T >::operator+=( Vec2T< U > const & rhs )noexcept
	{
		x = T( x + rhs.x );
		y = T( y + rhs.y );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec2T< T > & Vec2T< T >::operator-=( Vec2T< U > const & rhs )noexcept
	{
		x = T( x - rhs.x );
		y = T( y - rhs.y );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec2T< T > & Vec2T< T >::operator*=( Vec2T< U > const & rhs )noexcept
	{
		x = T( x * rhs.x );
		y = T( y * rhs.y );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec2T< T > & Vec2T< T >::operator+=( U const & rhs )noexcept
	{
		x = T( x + rhs );
		y = T( y + rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec2T< T > & Vec2T< T >::operator-=( U const & rhs )noexcept
	{
		x = T( x - rhs );
		y = T( y - rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec2T< T > & Vec2T< T >::operator*=( U const & rhs )noexcept
	{
		x = T( x * rhs );
		y = T( y * rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec2T< T > & Vec2T< T >::operator/=( U const & rhs )noexcept
	{
		x = T( x / rhs );
		y = T( y / rhs );
		return *this;
	}

	template< typename T >
	T dot( Vec2T< T > const & lhs, Vec2T< T > const & rhs )noexcept
	{
		return lhs.x * rhs.x
			+ lhs.y * rhs.y;
	}

	template< typename T >
	T length( Vec2T< T > const & vec )noexcept
	{
		return sqrt( dot( vec, vec ) );
	}

	template< typename T >
	T distance( Vec2T< T > const & lhs, Vec2T< T > const & rhs )noexcept
	{
		return length( rhs - lhs );
	}

	template< typename T >
	Vec2T< T > normalize( Vec2T< T > const & vec )noexcept
	{
		auto norm = length( vec );
		return Vec2T< T >
		{
			vec.x / norm,
			vec.y / norm
		};
	}

	template< typename T >
	inline bool operator==( Vec2T< T > const & lhs
		, Vec2T< T > const & rhs )noexcept
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y;
	}

	template< typename T >
	inline bool operator!=( Vec2T< T > const & lhs
		, Vec2T< T > const & rhs )noexcept
	{
		return lhs.x != rhs.x
			|| lhs.y != rhs.y;
	}
	
	template< typename T >
	inline Vec2T< T > operator-( Vec2T< T > const & rhs )noexcept
	{
		return Vec2T< T >
		{
			-rhs.x,
			-rhs.y
		};
	}

	template< typename T, typename U >
	inline Vec2T< T > operator+( Vec2T< T > const & lhs
		, Vec2T< U > const & rhs )noexcept
	{
		Vec2T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec2T< T > operator-( Vec2T< T > const & lhs
		, Vec2T< U > const & rhs )noexcept
	{
		Vec2T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec2T< T > operator*( Vec2T< T > const & lhs
		, Vec2T< U > const & rhs )noexcept
	{
		Vec2T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec2T< T > operator+( Vec2T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec2T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec2T< T > operator-( Vec2T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec2T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec2T< T > operator*( Vec2T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec2T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec2T< T > operator/( Vec2T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec2T< T > result{ lhs };
		result /= rhs;
		return result;
	}
}
