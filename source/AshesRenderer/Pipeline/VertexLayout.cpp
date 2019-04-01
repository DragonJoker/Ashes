/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Pipeline/VertexLayout.hpp"

#include "AshesRenderer/Shader/Attribute.hpp"

namespace ashes
{
	VertexLayout::VertexLayout( uint32_t bindingSlot
		, uint32_t stride
		, VertexInputRate inputRate )
		: m_bindingSlot{ bindingSlot }
		, m_stride{ stride }
		, m_inputRate{ inputRate }
	{
	}

	void VertexLayout::createAttribute( uint32_t location
		, Format format
		, uint32_t offset )
	{
		m_attributes.emplace_back( *this
			, format
			, location
			, offset );
	}

	void VertexLayout::createAttributes( uint32_t count
		, uint32_t startingLocation
		, Format format
		, uint32_t startingOffset )
	{
		for ( uint32_t i = 0u; i < count; ++i )
		{
			m_attributes.emplace_back( *this
				, format
				, startingLocation++
				, startingOffset );
			startingOffset += getSize( format );
		}
	}
}
