/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include <math.h>

#include "util/Vectorisation.hpp"

namespace utils
{
	template< typename T >
	QuaternionT< T >::QuaternionT( NoInit const & )noexcept
	{
	}

	template< typename T >
	constexpr QuaternionT< T >::QuaternionT()noexcept
		: x{}
		, y{}
		, z{}
		, w{ T{ 1 } }
	{
	}

	template< typename T >
	template< typename W
		, typename X
		, typename Y
		, typename Z >
	constexpr QuaternionT< T >::QuaternionT( W const & w
		, X const & x
		, Y const & y
		, Z const & z )noexcept
		: x{ T( x ) }
		, y{ T( y ) }
		, z{ T( z ) }
		, w{ T( w ) }
	{
	}
	
	template< typename T >
	QuaternionT< T >::QuaternionT( Vec3T< T > const & u, Vec3T< T > const & v )noexcept
	{
		Vec3T< T > const localW( cross( u, v ) );
		T dot = dot( u, v );
		QuaternionT< T > q( T{ 1 } + dot, localW.x, localW.y, localW.z );
		*this = normalize( q );
	}

	template< typename T >
	QuaternionT< T >::QuaternionT( Vec3T< RadiansT< T > > const & euler )noexcept
	{
		Vec3T< T > c{ vectorCall( utils::cos< T >, euler * T{ 0.5 } ) };
		Vec3T< T > s{ vectorCall( utils::sin< T >, euler * T{ 0.5 } ) };
		
		w = c.x * c.y * c.z + s.x * s.y * s.z;
		x = s.x * c.y * c.z - c.x * s.y * s.z;
		y = c.x * s.y * c.z + s.x * c.y * s.z;
		z = c.x * c.y * s.z - s.x * s.y * c.z;
	}

	template< typename T >
	template< typename U >
	QuaternionT< T >::QuaternionT( QuaternionT< U > const & rhs )noexcept
		: x{ T( rhs.x ) }
		, y{ T( rhs.y ) }
		, z{ T( rhs.z ) }
		, w{ T( rhs.w ) }
	{
	}

	template< typename T >
	template< typename U >
	QuaternionT< T > & QuaternionT< T >::operator=( QuaternionT< U > const & rhs )noexcept
	{
		x = T( rhs.x );
		y = T( rhs.y );
		z = T( rhs.z );
		w = T( rhs.w );
	}

	template< typename T >
	template< typename U >
	inline QuaternionT< T > & QuaternionT< T >::operator+=( QuaternionT< U > const & rhs )noexcept
	{
		x = T( x + rhs.x );
		y = T( y + rhs.y );
		z = T( z + rhs.z );
		w = T( w + rhs.w );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline QuaternionT< T > & QuaternionT< T >::operator-=( QuaternionT< U > const & rhs )noexcept
	{
		x = T( x - rhs.x );
		y = T( y - rhs.y );
		z = T( z - rhs.z );
		w = T( w - rhs.w );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline QuaternionT< T > & QuaternionT< T >::operator*=( QuaternionT< U > const & rhs )noexcept
	{
		QuaternionT< T > const p{ *this };

		w = p.w * rhs.w - p.x * rhs.x - p.y * rhs.y - p.z * rhs.z;
		x = p.w * rhs.x + p.x * rhs.w + p.y * rhs.z - p.z * rhs.y;
		y = p.w * rhs.y + p.y * rhs.w + p.z * rhs.x - p.x * rhs.z;
		z = p.w * rhs.z + p.z * rhs.w + p.x * rhs.y - p.y * rhs.x;

		return *this;
	}

	template< typename T >
	template< typename U >
	inline QuaternionT< T > & QuaternionT< T >::operator*=( U const & rhs )noexcept
	{
		x = T( x * rhs );
		y = T( y * rhs );
		z = T( z * rhs );
		w = T( w * rhs );
		return *this;
	}

	template< typename T >
	template< typename U >
	inline QuaternionT< T > & QuaternionT< T >::operator/=( U const & rhs )noexcept
	{
		x = T( x / rhs );
		y = T( y / rhs );
		z = T( z / rhs );
		w = T( w / rhs );
		return *this;
	}

	template< typename T >
	T dot( QuaternionT< T > const & lhs, QuaternionT< T > const & rhs )noexcept
	{
		return sqrt( lhs.x * rhs.x
			+ lhs.y * rhs.y
			+ lhs.z * rhs.z
			+ lhs.w * rhs.w );
	}

	template< typename T >
	QuaternionT< T > cross( QuaternionT< T > const & lhs, QuaternionT< T > const & rhs )noexcept
	{
		return QuaternionT< T > 
		{
			lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
			lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x
		};
	}

	template< typename T >
	T length( QuaternionT< T > const & quat )noexcept
	{
		return dot( quat, quat );
	}

	template< typename T >
	T distance( QuaternionT< T > const & lhs, QuaternionT< T > const & rhs )noexcept
	{
		return length( rhs - lhs );
	}

	template< typename T >
	QuaternionT< T > normalize( QuaternionT< T > const & vec )noexcept
	{
		auto norm = length( vec );
		return QuaternionT< T >
		{
			vec.x / norm,
			vec.y / norm,
			vec.z / norm,
			vec.w / norm
		};
	}
	
	template <typename T >
	QuaternionT< T > conjugate( QuaternionT< T > const & q )noexcept
	{
		return QuaternionT< T >( q.w, -q.x, -q.y, -q.z );
	}

	template <typename T >
	QuaternionT< T > inverse( QuaternionT< T > const & q )noexcept
	{
		return conjugate( q ) / dot( q, q );
	}

	template< typename T, typename U >
	Vec3T< T > rotate( QuaternionT< T > const & q, Vec3T< U > const & v )noexcept
	{
		return q * v;
	}

	template< typename T, typename U >
	Vec4T< T > rotate( QuaternionT< T > const & q, Vec4T< U > const & v )noexcept
	{
		return q * v;
	}

	template< typename T >
	QuaternionT< T > rotate( QuaternionT< T > const & q,
		RadiansT < T > const & angle,
		Vec3T< T > const & axis )noexcept
	{
		assert( std::abs( length( axis ) - T( 1 ) ) <= T( 0.001 ) );
		T const sinA = sin( angle * T( 0.5 ) );
		return q * QuaternionT< T >{ cos( angle * T( 0.5 ) )
			, axis.x * sinA
			, axis.y * sinA
			, axis.z * sinA };
	}

	template< typename T >
	QuaternionT< T > pitch( QuaternionT< T > const & q,
		RadiansT < T > const & angle )noexcept
	{
		return rotate( q, angle, Vec3T< T >{ T{ 1 }, T{}, T{} } );
	}

	template< typename T >
	QuaternionT< T > yaw( QuaternionT< T > const & q,
		RadiansT < T > const & angle )noexcept
	{
		return rotate( q, angle, Vec3T< T >{ T{}, T{ 1 }, T{} } );
	}

	template< typename T >
	QuaternionT< T > roll( QuaternionT< T > const & q,
		RadiansT < T > const & angle )noexcept
	{
		return rotate( q, angle, Vec3T< T >{ T{}, T{}, T{ 1 } } );
	}

	template< typename T >
	Mat4T< T > toMat4( QuaternionT< T > const & q )noexcept
	{
		Mat4T< T > result{ T{ 1 } };
		T qxx{ q.x * q.x };
		T qyy{ q.y * q.y };
		T qzz{ q.z * q.z };
		T qxz{ q.x * q.z };
		T qxy{ q.x * q.y };
		T qyz{ q.y * q.z };
		T qwx{ q.w * q.x };
		T qwy{ q.w * q.y };
		T qwz{ q.w * q.z };

		result[0][0] = T{ 1 } - T{ 2 } * ( qyy +  qzz );
		result[0][1] = T{ 2 } * ( qxy + qwz );
		result[0][2] = T{ 2 } * ( qxz - qwy );

		result[1][0] = T{ 2 } * ( qxy - qwz );
		result[1][1] = T{ 1 } - T{ 2 } * ( qxx +  qzz );
		result[1][2] = T{ 2 } * ( qyz + qwx );

		result[2][0] = T{ 2 } * ( qxz + qwy );
		result[2][1] = T{ 2 } * ( qyz - qwx );
		result[2][2] = T{ 1 } - T{ 2 } * ( qxx +  qyy );

		return result;
	}

	template< typename T >
	inline bool operator==( QuaternionT< T > const & lhs
		, QuaternionT< T > const & rhs )noexcept
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.z == rhs.z
			&& lhs.w == rhs.w;
	}

	template< typename T >
	inline bool operator!=( QuaternionT< T > const & lhs
		, QuaternionT< T > const & rhs )noexcept
	{
		return lhs.x != rhs.x
			|| lhs.y != rhs.y
			|| lhs.z != rhs.z
			|| lhs.w != rhs.w;
	}

	template< typename T, typename U >
	inline QuaternionT< T > operator+( QuaternionT< T > const & lhs
		, QuaternionT< U > const & rhs )noexcept
	{
		QuaternionT< T > result{ lhs };
		result += rhs;
		return result;
	}

	template< typename T, typename U >
	inline QuaternionT< T > operator-( QuaternionT< T > const & lhs
		, QuaternionT< U > const & rhs )noexcept
	{
		QuaternionT< T > result{ lhs };
		result -= rhs;
		return result;
	}

	template< typename T, typename U >
	inline QuaternionT< T > operator*( QuaternionT< T > const & lhs
		, QuaternionT< U > const & rhs )noexcept
	{
		QuaternionT< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline QuaternionT< T > operator*( QuaternionT< T > const & lhs
		, U const & rhs )noexcept
	{
		QuaternionT< T > result{ lhs };
		result *= rhs;
		return result;
	}

	template< typename T, typename U >
	inline QuaternionT< T > operator/( QuaternionT< T > const & lhs
		, U const & rhs )noexcept
	{
		QuaternionT< T > result{ lhs };
		result /= rhs;
		return result;
	}

	template< typename T, typename U >
	inline Vec3T< T > operator*( QuaternionT< T > const & lhs
		, Vec3T< U > const & rhs )noexcept
	{
		Vec3T< T > const quatVector{ lhs.x, lhs.y, lhs.z  };
		Vec3T< T > const uv{ cross( quatVector, rhs ) };
		Vec3T< T > const uuv{ cross( quatVector, uv ) };

		return rhs + ( ( uv * lhs.w ) + uuv ) * static_cast< T >( 2 );
	}

	template< typename T, typename U >
	inline Vec4T< T > operator*( QuaternionT< T > const & lhs
		, Vec4T< U > const & rhs )noexcept
	{
		return Vec4T< T >
		{
			lhs.x * rhs.x,
			lhs.y * rhs.y,
			lhs.z * rhs.z,
			lhs.w * rhs.w
		};
	}
}
