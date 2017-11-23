#include "Mesh.h"

#include "Submesh.h"

namespace render
{
	void Mesh::data( renderer::RenderingResources const & resources
		, Vec3Array const & pos
		, Vec3Array const & nml
		, Vec2Array const & tex )
	{
		m_positions = renderer::makeVertexBuffer< utils::Vec3 >( resources.getDevice()
			, 0u
			, uint32_t( pos.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		resources.getStagingBuffer().copyVertexData( resources.getCommandBuffer()
			, pos
			, *m_positions );

		if ( !nml.empty() )
		{
			m_normal = renderer::makeVertexBuffer< utils::Vec3 >( resources.getDevice()
				, 1u
				, uint32_t( nml.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			resources.getStagingBuffer().copyVertexData( resources.getCommandBuffer()
				, nml
				, *m_normal );
		}

		if ( !tex.empty() )
		{
			m_texcoord = renderer::makeVertexBuffer< utils::Vec2 >( resources.getDevice()
				, 2u
				, uint32_t( tex.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			resources.getStagingBuffer().copyVertexData( resources.getCommandBuffer()
				, tex
				, *m_texcoord );
		}

		utils::Vec3 min
		{
			std::numeric_limits< float >::max(),
			std::numeric_limits< float >::max(),
			std::numeric_limits< float >::max()
		};
		utils::Vec3 max
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

		m_boundaries = utils::vectorCall< float, float >( std::abs, max - min );
	}

	void Mesh::addSubmesh( renderer::RenderingResources const & resources
		, UInt16Array const & idx )
	{
		m_submeshes.push_back( std::make_shared< Submesh >( resources, *this, idx ) );
	}
}
