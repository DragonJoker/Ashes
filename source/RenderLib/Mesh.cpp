#include "Mesh.h"

#include "Submesh.h"

#include <GlLib/GlBuffer.h>

namespace render
{
	void Mesh::data( Vec3Array const & pos
		, Vec3Array const & nml
		, Vec2Array const & tex )
	{
		m_positions = gl::makeBuffer( gl::BufferTarget::eArrayBuffer
			, pos );

		if ( !nml.empty() )
		{
			m_normal = gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, nml );
		}

		if ( !tex.empty() )
		{
			m_texcoord = gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, tex );
		}

		gl::Vec3 min
		{
			std::numeric_limits< float >::max(),
			std::numeric_limits< float >::max(),
			std::numeric_limits< float >::max()
		};
		gl::Vec3 max
		{
			std::numeric_limits< float >::lowest(),
			std::numeric_limits< float >::lowest(),
			std::numeric_limits< float >::lowest()
		};

		for ( auto & position : pos )
		{
			min.x = std::min( min.x, position.x );
			min.y = std::min( min.y, position.y );
			min.z = std::min( min.z, position.z );
			max.x = std::max( max.x, position.x );
			max.y = std::max( max.y, position.y );
			max.z = std::max( max.z, position.z );
		}

		m_boundaries = gl::vectorCall< float, float >( std::abs, max - min );
	}

	void Mesh::addSubmesh( UInt16Array const & idx )
	{
		m_submeshes.push_back( std::make_shared< Submesh >( *this, idx ) );
	}
}
