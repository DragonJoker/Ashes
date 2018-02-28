/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Buffer/GeometryBuffers.hpp"

#include "Pipeline/VertexInputState.hpp"

namespace renderer
{
	namespace
	{
		VertexInputAttributeDescriptionArray getAttributes( VertexInputAttributeDescriptionArray const & attributes
			, uint32_t bindingSlot )
		{
			VertexInputAttributeDescriptionArray result;

			for ( auto & attribute : attributes )
			{
				if ( attribute.binding == bindingSlot )
				{
					result.push_back( attribute );
				}
			}

			return result;
		}
	}

	GeometryBuffers::GeometryBuffers( VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > vboOffsets
		, VertexInputState const & vertexInputState )
	{
		assert( vbos.size() == vboOffsets.size() );
		assert( vbos.size() == vertexInputState.vertexBindingDescriptions.size() );
		m_vbos.reserve( vbos.size() );

		for ( auto i = 0u; i < vbos.size(); ++i )
		{
			m_vbos.push_back(
			{
				vbos[i].get(),
				vboOffsets[i],
				vertexInputState.vertexBindingDescriptions[i],
				getAttributes( vertexInputState.vertexAttributeDescriptions
					, vertexInputState.vertexBindingDescriptions[i].binding )
			} );
		}
	}

	GeometryBuffers::GeometryBuffers( VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > vboOffsets
		, VertexInputState const & vertexInputState
		, BufferBase const & ibo
		, uint64_t iboOffset
		, IndexType type )
		: m_ibo{ std::make_unique< IBO >( ibo, iboOffset, type ) }
	{
		assert( vbos.size() == vboOffsets.size() );
		assert( vbos.size() == vertexInputState.vertexBindingDescriptions.size() );
		m_vbos.reserve( vbos.size() );

		for ( auto i = 0u; i < vbos.size(); ++i )
		{
			m_vbos.push_back(
			{
				vbos[i].get(),
				vboOffsets[i],
				vertexInputState.vertexBindingDescriptions[i],
				getAttributes( vertexInputState.vertexAttributeDescriptions
					, vertexInputState.vertexBindingDescriptions[i].binding )
			} );
		}
	}
}
