/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
namespace utils
{
	template< typename T >
	Vec4T< T >::Vec4T( NoInit const & )noexcept
	{
	}

	template< typename T >
	constexpr Vec4T< T >::Vec4T()noexcept
		: x{}
		, y{}
		, z{}
		, w{}
	{
	}

	template< typename T >
	template< typename X
		, typename Y
		, typename Z
		, typename W >
	constexpr Vec4T< T >::Vec4T( X const & x
		, Y const & y
		, Z const & z
		, W const & w )noexcept
		: x{ T( x ) }
		, y{ T( y ) }
		, z{ T( z ) }
		, w{ T( w ) }
	{
	}

	template< typename T >
	template< typename X
		, typename YZW >
	constexpr Vec4T< T >::Vec4T( X const & x
		, Vec3T< YZW > const & yzw )noexcept
		: x{ T( x ) }
		, y{ T( yzw.x ) }
		, z{ T( yzw.y ) }
		, w{ T( yzw.z ) }
	{
	}

	template< typename T >
	template< typename XYZ
		, typename W >
	constexpr Vec4T< T >::Vec4T( Vec3T< XYZ > const & xyz
		, W const & w )noexcept
		: x{ T( xyz.x ) }
		, y{ T( xyz.y ) }
		, z{ T( xyz.z ) }
		, w{ T( w ) }
	{
	}

	template< typename T >
	template< typename XY
		, typename ZW >
	constexpr Vec4T< T >::Vec4T( Vec2T< XY > const & xy
		, Vec2T< ZW > const & zw )noexcept
		: x{ T( xy.x ) }
		, y{ T( xy.y ) }
		, z{ T( zw.x ) }
		, w{ T( zw.y ) }
	{
	}

	template< typename T >
	template< typename XY
		, typename Z
		, typename W >
	constexpr Vec4T< T >::Vec4T( Vec2T< XY > const & xy
		, Z const & z
		, W const & w )noexcept
		: x{ T( xy.x ) }
		, y{ T( xy.y ) }
		, z{ T( z ) }
		, w{ T( w ) }
	{
	}

	template< typename T >
	template< typename X
		, typename YZ
		, typename W >
	constexpr Vec4T< T >::Vec4T( X const & x
		, Vec2T< YZ > const & yz
		, W const & w )noexcept
		: x{ T( x ) }
		, y{ T( yz.x ) }
		, z{ T( yz.y ) }
		, w{ T( w ) }
	{
	}

	template< typename T >
	template< typename X
		, typename Y
		, typename ZW >
	constexpr Vec4T< T >::Vec4T( X const & x
		, X const & y
		, Vec2T< ZW > const & zw )noexcept
		: x{ T( x ) }
		, y{ T( y ) }
		, z{ T( zw.x ) }
		, w{ T( zw.y ) }
	{
	}

	template< typename T >
	constexpr Vec4T< T >::Vec4T( T const & v )noexcept
		: x{ v }
		, y{ v }
		, z{ v }
		, w{ v }
	{
	}

	template< typename T >
	template< typename U >
	Vec4T< T >::Vec4T( Vec4T< U > const & rhs )noexcept
		: x{ T( rhs.x ) }
		, y{ T( rhs.y ) }
		, z{ T( rhs.z ) }
		, w{ T( rhs.w ) }
	{
	}

	template< typename T >
	template< typename U >
	Vec4T< T > & Vec4T< T >::operator=( Vec4T< U > const & rhs )noexcept
	{
		x = T( rhs.x );
		y = T( rhs.y );
		z = T( rhs.z );
		w = T( rhs.w );
	}

	template< typename T >
	T & Vec4T< T >::operator[]( size_t index )noexcept
	{
		assert( index < 4 );
		return data[index];
	}

	template< typename T >
	T const & Vec4T< T >::operator[]( size_t index )const noexcept
	{
		assert( index < 4 );
		return data[index];
	}

	template< typename T >
	inline Vec4T< T > operator-( Vec4T< T > const & rhs )noexcept
	{
		return Vec4T< T >
		{
			-rhs.x,
			-rhs.y,
			-rhs.z,
			-rhs.w
		};
	}

	template< typename T >
	template< typename U >
	inline Vec4T< T > & Vec4T< T >::operator+=( Vec4T< U > const & rhs )noexcept
	{
		x = T( x + rhs.x );
		y = T( y + rhs.y );
		z = T( z + rhs.z );
		w = T( w + rhs.w );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec4T< T > & Vec4T< T >::operator-=( Vec4T< U > const & rhs )noexcept
	{
		x = T( x - rhs.x );
		y = T( y - rhs.y );
		z = T( z - rhs.z );
		w = T( w - rhs.w );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec4T< T > & Vec4T< T >::operator*=( Vec4T< U > const & rhs )noexcept
	{
		x = T( x * rhs.x );
		y = T( y * rhs.y );
		z = T( z * rhs.z );
		w = T( w * rhs.w );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec4T< T > & Vec4T< T >::operator+=( U const & rhs )noexcept
	{
		x = T( x + rhs );
		y = T( y + rhs );
		z = T( z + rhs );
		w = T( w + rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec4T< T > & Vec4T< T >::operator-=( U const & rhs )noexcept
	{
		x = T( x - rhs );
		y = T( y - rhs );
		z = T( z - rhs );
		w = T( w - rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec4T< T > & Vec4T< T >::operator*=( U const & rhs )noexcept
	{
		x = T( x * rhs );
		y = T( y * rhs );
		z = T( z * rhs );
		w = T( w * rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline Vec4T< T > & Vec4T< T >::operator/=( U const & rhs )noexcept
	{
		x = T( x / rhs );
		y = T( y / rhs );
		z = T( z / rhs );
		w = T( w / rhs );
		return *this;
	}

	template< typename T >
	T dot( Vec4T< T > const & lhs, Vec4T< T > const & rhs )noexcept
	{
		return lhs.x * rhs.x
			+ lhs.y * rhs.y
			+ lhs.z * rhs.z
			+ lhs.w * rhs.w;
	}

	template< typename T >
	T length( Vec4T< T > const & vec )noexcept
	{
		return sqrt( dot( vec, vec ) );
	}

	template< typename T >
	T distance( Vec4T< T > const & lhs, Vec4T< T > const & rhs )noexcept
	{
		return length( rhs - lhs );
	}

	template< typename T >
	Vec4T< T > normalize( Vec4T< T > const & vec )noexcept
	{
		auto norm = length( vec );
		return Vec4T< T >
		{
			vec.x / norm,
			vec.y / norm,
			vec.z / norm,
			vec.w / norm
		};
	}

	template< typename T >
	inline bool operator==( Vec4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.z == rhs.z
			&& lhs.w == rhs.w;
	}

	template< typename T >
	inline bool operator!=( Vec4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept
	{
		return lhs.x != rhs.x
			|| lhs.y != rhs.y
			|| lhs.z != rhs.z
			|| lhs.w != rhs.w;
	}

	template< typename T, typename U >
	inline Vec4T< T > operator+( Vec4T< T > const & lhs
		, Vec4T< U > const & rhs )noexcept
	{
		Vec4T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec4T< T > operator-( Vec4T< T > const & lhs
		, Vec4T< U > const & rhs )noexcept
	{
		Vec4T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec4T< T > operator*( Vec4T< T > const & lhs
		, Vec4T< U > const & rhs )noexcept
	{
		Vec4T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec4T< T > operator+( Vec4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec4T< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec4T< T > operator-( Vec4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec4T< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec4T< T > operator*( Vec4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec4T< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec4T< T > operator/( Vec4T< T > const & lhs
		, U const & rhs )noexcept
	{
		Vec4T< T > result{ lhs };
		result /= rhs;
		return result;
	}
}
