/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#include <cassert>
#include <cmath>

namespace utils
{
	template< typename T >
	Vec3T< T >::Vec3T( NoInit const & )noexcept
	{
	}

	template< typename T >
	constexpr Vec3T< T >::Vec3T()noexcept
		: x{}
		, y{}
		, z{}
	{
	}

	template< typename T >
	template< typename X
		, typename Y
		, typename Z >
	constexpr Vec3T< T >::Vec3T( X const & x
		, Y const & y
		, Z const & z )noexcept
		: x{ T( x ) }
		, y{ T( y ) }
		, z{ T( z ) }
	{
	}

	template< typename T >
	constexpr Vec3T< T >::Vec3T( T const & v )noexcept
		: x{ v }
		, y{ v }
		, z{ v }
	{
	}

	template< typename T >
	constexpr Vec3T< T >::Vec3T( Vec2T< T > const & xy, T const & z )noexcept
		: x{ xy.x }
		, y{ xy.y }
		, z{ z }
	{
	}

	template< typename T >
	constexpr Vec3T< T >::Vec3T( T const & x, Vec2T< T > const & yz )noexcept
		: x{ x }
		, y{ yz.x }
		, z{ yz.y }
	{
	}

	template< typename T >
	template< typename U >
	Vec3T< T >::Vec3T( Vec3T< U > const & rhs )noexcept
		: x{ T( rhs.x ) }
		, y{ T( rhs.y ) }
		, z{ T( rhs.z ) }
	{
	}

	template< typename T >
	template< typename U >
	Vec3T< T > & Vec3T< T >::operator=( Vec3T< U > const & rhs )noexcept
	{
		x = T( rhs.x );
		y = T( rhs.y );
		z = T( rhs.z );
	}

	template< typename T >
	T & Vec3T< T >::operator[]( size_t index )noexcept
	{
		assert( index < 3 );
		return data[index];
	}

	template< typename T >
	T const & Vec3T< T >::operator[]( size_t index )const noexcept
	{
		assert( index < 3 );
		return data[index];
	}

	template< typename T >
	template< typename U >
	inline Vec3T< T > & Vec3T< T >::operator+=( Vec3T< U > const & rhs )noexcept
	{
		x = T( x + rhs.x );
		y = T( y + rhs.y );
		z = T( z + rhs.z );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec3T< T > & Vec3T< T >::operator-=( Vec3T< U > const & rhs )noexcept
	{
		x = T( x - rhs.x );
		y = T( y - rhs.y );
		z = T( z - rhs.z );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec3T< T > & Vec3T< T >::operator*=( Vec3T< U > const & rhs )noexcept
	{
		x = T( x * rhs.x );
		y = T( y * rhs.y );
		z = T( z * rhs.z );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec3T< T > & Vec3T< T >::operator+=( U const & rhs )noexcept
	{
		x = T( x + rhs );
		y = T( y + rhs );
		z = T( z + rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec3T< T > & Vec3T< T >::operator-=( U const & rhs )noexcept
	{
		x = T( x - rhs );
		y = T( y - rhs );
		z = T( z - rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec3T< T > & Vec3T< T >::operator*=( U const & rhs )noexcept
	{
		x = T( x * rhs );
		y = T( y * rhs );
		z = T( z * rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec3T< T > & Vec3T< T >::operator/=( U const & rhs )noexcept
	{
		x = T( x / rhs );
		y = T( y / rhs );
		z = T( z / rhs );
		return *this;
	}

	template< typename T >
	T dot( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept
	{
		return lhs.x * rhs.x
			+ lhs.y * rhs.y
			+ lhs.z * rhs.z;
	}

	template< typename T >
	Vec3T< T > cross( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept
	{
		return Vec3T< T>
		{
			lhs.y * rhs.z - rhs.y * lhs.z,
			lhs.z * rhs.x - rhs.z * lhs.x,
			lhs.x * rhs.y - rhs.x * lhs.y 
		};
	}

	template< typename T >
	T length( Vec3T< T > const & vec )noexcept
	{
		return sqrt( dot( vec, vec ) );
	}

	template< typename T >
	T distance( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept
	{
		return length( rhs - lhs );
	}

	template< typename T >
	Vec3T< T > normalize( Vec3T< T > const & vec )noexcept
	{
		auto norm = length( vec );
		return Vec3T< T >
		{
			vec.x / norm,
			vec.y / norm,
			vec.z / norm
		};
	}

	template< typename T >
	Vec2T< T > toVec2( Vec3T< T > const & vec )noexcept
	{
		return Vec2T< T >{ vec.x, vec.y };
	}

	template< typename T >
	inline bool operator==( Vec3T< T > const & lhs
		, Vec3T< T > const & rhs )noexcept
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.z == rhs.z;
	}

	template< typename T >
	inline bool operator!=( Vec3T< T > const & lhs
		, Vec3T< T > const & rhs )noexcept
	{
		return lhs.x != rhs.x
			|| lhs.y != rhs.y
			|| lhs.z != rhs.z;
	}

	template< typename T >
	inline Vec3T< T > operator-( Vec3T< T > const & rhs )noexcept
	{
		return Vec3T< T >
		{
			-rhs.x,
			-rhs.y,
			-rhs.z
		};
	}

	template< typename T, typename U >
	inline Vec3T< T > operator+( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept
	{
		Vec3T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec3T< T > operator-( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept
	{
		Vec3T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec3T< T > operator*( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept
	{
		Vec3T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec3T< T > operator+( Vec3T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec3T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec3T< T > operator-( Vec3T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec3T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec3T< T > operator*( Vec3T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec3T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec3T< T > operator/( Vec3T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec3T< T > result{ lhs };
		result /= rhs;
		return result;
	}
}
