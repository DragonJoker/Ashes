/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#include <math.h>

namespace utils
{
	template< typename T >
	renderer::Mat4T< T > translate( renderer::Mat4T< T > const & m
		, Vec3T< T > const & v )
	{
		renderer::Mat4T< T > result{ m };
		result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return result;
	}

	template< typename T >
	renderer::Mat4T< T > scale( renderer::Mat4T< T > const & m
		, Vec3T< T > const & v )
	{
		renderer::Mat4T< T > result{ renderer::noInit };
		result[0] = m[0] * v[0];
		result[1] = m[1] * v[1];
		result[2] = m[2] * v[2];
		result[3] = m[3];
		return result;
	}

	template< typename T >
	renderer::Mat4T< T > rotate( renderer::Mat4T< T > const & m
		, T angle
		, Vec3T< T > const & v )
	{
		T const a = angle;
		T const c = T( std::cos( a ) );
		T const s = T( std::sin( a ) );

		Vec3T< T > axis( normalize( v ) );
		Vec3T< T > temp( axis * ( T( 1 ) - c ) );

		renderer::Mat4T< T > rotate{ renderer::noInit };
		rotate[0][0] = c + temp[0] * axis[0];
		rotate[0][1] = temp[0] * axis[1] + s * axis[2];
		rotate[0][2] = temp[0] * axis[2] - s * axis[1];

		rotate[1][0] = temp[1] * axis[0] - s * axis[2];
		rotate[1][1] = c + temp[1] * axis[1];
		rotate[1][2] = temp[1] * axis[2] + s * axis[0];

		rotate[2][0] = temp[2] * axis[0] + s * axis[1];
		rotate[2][1] = temp[2] * axis[1] - s * axis[0];
		rotate[2][2] = c + temp[2] * axis[2];

		renderer::Mat4T< T > result( renderer::noInit );
		result[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
		result[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
		result[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
		result[3] = m[3];
		return result;
	}

	template< typename T >
	renderer::Mat4T< T > lookAt( Vec3T< T > const & eye
		, Vec3T< T > const & center
		, Vec3T< T > const & up )
	{
		Vec3T< T > const f{ normalize( center - eye ) };
		Vec3T< T > const s{ normalize( cross( f, up ) ) };
		Vec3T< T > const u{ cross( s, f ) };
		
		renderer::Mat4T< T > result{ T{ 1 } };
		result[0][0] = s.x;
		result[1][0] = s.y;
		result[2][0] = s.z;
		result[0][1] = u.x;
		result[1][1] = u.y;
		result[2][1] = u.z;
		result[0][2] =-f.x;
		result[1][2] =-f.y;
		result[2][2] =-f.z;
		result[3][0] =-dot( s, eye );
		result[3][1] =-dot( u, eye );
		result[3][2] = dot( f, eye );
		return result;
	}

	template< typename T >
	renderer::Mat4T< T > perspective( RadiansT< T > fovy
		, T aspect
		, T zNear
		, T zFar )
	{
		T const tanHalfFovy = tan( fovy / static_cast< T >( 2 ) );

		renderer::Mat4T< T > result{ T{ 0 } };
		result[0][0] = static_cast< T >( 1 ) / ( aspect * tanHalfFovy );
		result[1][1] = static_cast< T >( 1 ) / ( tanHalfFovy );
		result[2][3] = -static_cast< T >( 1 );
		// [0, 1]
		result[2][2] = zFar / ( zNear - zFar );
		result[3][2] = -( zFar * zNear ) / ( zFar - zNear );
		//// [-1, 1]
		//result[2][2] = ( zFar + zNear ) / ( zFar - zNear );
		//result[3][2] = -( static_cast< T >( 2 ) * zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	template< typename T >
	renderer::Mat4T< T > infinitePerspective( RadiansT< T > fovy
		, T aspect
		, T zNear )
	{
		T const range = tan( fovy / static_cast< T >( 2 ) ) * zNear;
		T const left = -range * aspect;
		T const right = range * aspect;
		T const bottom = -range;
		T const top = range;

		renderer::Mat4T< T > result{ T{ 0 } };
		result[0][0] = ( static_cast< T >( 2 ) * zNear ) / ( right - left );
		result[1][1] = ( static_cast< T >( 2 ) * zNear ) / ( top - bottom );
		result[2][2] = -static_cast< T >( 1 );
		result[2][3] = -static_cast< T >( 1 );
		result[3][2] = -static_cast< T >( 2 ) * zNear;
		return result;
	}

	template< typename T >
	renderer::Mat4T< T > ortho( T left
		, T right
		, T bottom
		, T top
		, T zNear
		, T zFar )
	{
		renderer::Mat4T< T > result{ 1 };
		result[0][0] = static_cast< T >( 2 ) / ( right - left );
		result[1][1] = static_cast< T >( 2 ) / ( top - bottom );
		result[3][0] = -( right + left ) / ( right - left );
		result[3][1] = -( top + bottom ) / ( top - bottom );
		// [0, 1]
		result[2][2] = -static_cast< T >( 1 ) / ( zFar - zNear );
		result[3][2] = -zNear / ( zFar - zNear );
		//// [-1, 1]
		//result[2][2] = - static_cast< T >( 2 ) / ( zFar - zNear );
		//result[3][2] = - ( zFar + zNear ) / ( zFar - zNear );

		return result;
	}

	template< typename T, typename U >
	Vec3T< T > project( Vec3T< T > const & obj
		, renderer::Mat4T< T > const & model
		, renderer::Mat4T< T > const & proj
		, renderer::Vec4T< U > const & viewport )
	{
		renderer::Vec4T< T > tmp{ obj.x, obj.y, obj.z, static_cast< T >( 1 ) };
		tmp = model * tmp;
		tmp = proj * tmp;

		tmp /= tmp.w;
		tmp = tmp * static_cast< T >( 0.5 ) + static_cast< T >( 0.5 );
		tmp[0] = tmp[0] * T( viewport[2] ) + T( viewport[0] );
		tmp[1] = tmp[1] * T( viewport[3] ) + T( viewport[1] );

		return Vec3T< T >{ tmp.x, tmp.y, tmp.z };
	}
}
