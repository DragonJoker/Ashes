#include "Mesh.h"

#include "Submesh.h"

#include <Renderer/StagingBuffer.hpp>

namespace render
{
	void Mesh::data( renderer::Device const & device
		, renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Vec3Array const & pos
		, Vec3Array const & nml
		, Vec2Array const & tex )
	{
		m_positions = renderer::makeVertexBuffer< renderer::Vec3 >( device
			, uint32_t( pos.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		stagingBuffer.copyVertexData( commandBuffer
			, pos
			, *m_positions
			, renderer::PipelineStageFlag::eVertexInput );

		if ( !nml.empty() )
		{
			m_normal = renderer::makeVertexBuffer< renderer::Vec3 >( device
				, uint32_t( nml.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			stagingBuffer.copyVertexData( commandBuffer
				, nml
				, *m_normal
				, renderer::PipelineStageFlag::eVertexInput );
		}

		if ( !tex.empty() )
		{
			m_texcoord = renderer::makeVertexBuffer< renderer::Vec2 >( device
				, uint32_t( tex.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			stagingBuffer.copyVertexData( commandBuffer
				, tex
				, *m_texcoord
				, renderer::PipelineStageFlag::eVertexInput );
		}

		renderer::Vec3 min
		{
			std::numeric_limits< float >::max(),
			std::numeric_limits< float >::max(),
			std::numeric_limits< float >::max()
		};
		renderer::Vec3 max
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

	void Mesh::addSubmesh( renderer::Device const & device
		, renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, UInt16Array const & idx )
	{
		m_submeshes.push_back( std::make_shared< Submesh >( device
			, stagingBuffer
			, commandBuffer
			, *this
			, idx ) );
	}
}
