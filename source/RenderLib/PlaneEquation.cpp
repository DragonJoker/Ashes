#include "PlaneEquation.h"

namespace render
{
	PlaneEquation::PlaneEquation()
		: m_d{ 0 }
	{
	}

	PlaneEquation::PlaneEquation( gl::Vec3 const & p1
		, gl::Vec3 const & p2
		, gl::Vec3 const & p3 )
		: m_d{ 0 }
	{
		set( p1, p2, p3 );
	}

	PlaneEquation::PlaneEquation( gl::Vec3 const & normal
		, gl::Vec3 const & point )
		: m_normal{ normal }
		, m_d{ 0 }
	{
		set( normal, point );
	}

	PlaneEquation::~PlaneEquation()
	{
	}

	void PlaneEquation::set( gl::Vec3 const & p1
		, gl::Vec3 const & p2
		, gl::Vec3 const & p3 )
	{
		gl::Vec3 v{ p2 - p1 };
		gl::Vec3 w{ p3 - p1 };
		m_normal = gl::normalize( gl::cross( w, v ) );
		m_point = ( p1 + p2 + p3 ) / float( 3 );
		m_d = -gl::dot( m_point, m_normal );
	}

	void PlaneEquation::set( gl::Vec3 const & normal
		, gl::Vec3 const & point )
	{
		m_normal = gl::normalize( normal );
		m_point = point;
		m_d = -gl::dot( m_point, m_normal );
	}

	bool PlaneEquation::parallel( PlaneEquation const & plane )const
	{
		float ratioA = m_normal[0] / plane.m_normal[0];
		float ratioB = m_normal[1] / plane.m_normal[1];
		float ratioC = m_normal[2] / plane.m_normal[2];
		return ratioA == ratioB
			   && ratioA == ratioC;
	}

	float PlaneEquation::distance( gl::Vec3 const & point )const
	{
		return gl::dot( m_normal, point ) + m_d;
	}

	gl::Vec3 PlaneEquation::project( gl::Vec3 const & point )const
	{
		return point - ( m_normal * distance( point ) );
	}

	bool PlaneEquation::intersects( PlaneEquation const & plane )const
	{
		bool result = false;

		if ( ! parallel( plane ) )
		{
			float constexpr def{};
			float b1 = m_normal[1];
			float c1 = m_normal[2];
			float d1 = m_d;
			float b2 = plane.m_normal[1];
			float c2 = plane.m_normal[2];
			float d2 = m_d;
			float div = ( b1 * c2 ) - ( b2 * c1 );

			if ( b1 != def && div != def )
			{
				gl::Vec3 point;
				point[2] = ( ( b2 * d1 ) - ( b1 * d2 ) ) / div;
				point[1] = ( ( -c1 * point[2] ) - d1 ) / b1;
				point[0] = 0;
			}

			result = true;
		}

		return result;
	}

	bool PlaneEquation::intersects( PlaneEquation const & plane1
		, PlaneEquation const & plane2
		, gl::Vec3 & intersection )const
	{
		bool result = false;

		if ( !parallel( plane1 ) && !parallel( plane2 ) && !plane1.parallel( plane2 ) )
		{
			float a1 = m_normal[0], a2 = plane1.m_normal[0], a3 = plane2.m_normal[0];
			float b1 = m_normal[1], b2 = plane1.m_normal[1], b3 = plane2.m_normal[1];
			float c1 = m_normal[2], c2 = plane1.m_normal[2], c3 = plane2.m_normal[2];
			float d = m_d;
			float d1 = plane1.m_d;
			float d2 = plane2.m_d;
			float alpha, beta;
			alpha = ( a3 - ( a2 * ( b3 - ( a3 / a1 ) ) / ( b2 - ( a2 / a1 ) ) ) ) / a1;
			beta = ( b3 - ( a3 / a1 ) ) / ( b2 - ( a2 / a1 ) );
			float c3b = ( c1 * alpha ) + ( c2 * beta );

			if ( c3 != c3b )
			{
				alpha = ( ( a2 * c1 ) / ( a1 * ( b2 - ( a2 * b1 ) / a1 ) ) ) - ( c2 / ( b2 - ( a2 * b1 ) / a1 ) );
				beta = ( ( a2 * d ) / ( a1 * ( b2 - ( a2 * b1 ) / a1 ) ) ) - ( d1 / ( b2 - ( a2 * b1 ) / a1 ) );
				float x, y, z;
				z = ( ( a3 * ( ( d + ( beta * b1 ) ) / a1 ) ) - d2 ) / ( ( b3 * alpha ) + c3 - ( a3 * ( ( alpha * b1 ) + c1 ) / a1 ) );
				y = ( alpha * z ) + beta;
				x = ( z * ( 0.0f - ( ( alpha * b1 ) + c1 ) ) / a1 ) - ( ( d + ( b1 * beta ) ) / a1 );
				intersection[0] = x;
				intersection[1] = y;
				intersection[2] = z;
				result = true;
			}
		}

		return result;
	}

	bool operator==( PlaneEquation const & lhs, PlaneEquation const & rhs )
	{
		bool result = false;

		if ( lhs.parallel( rhs ) )
		{
			float ratioA = lhs.m_normal[0] / rhs.m_normal[0];
			float ratioD = lhs.m_d / rhs.m_d;
			result = ratioA == ratioD;
		}

		return result;
	}

	bool operator!=( PlaneEquation const & a, PlaneEquation const & b )
	{
		return ( !( a == b ) );
	}
}
