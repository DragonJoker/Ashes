/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Pipeline/VertexLayout.hpp"

#include "Shader/Attribute.hpp"

namespace renderer
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
		, uint32_t offset
		, std::string const & semanticName
		, uint32_t semanticIndex )
	{
		m_attributes.emplace_back( *this
			, format
			, location
			, offset
			, semanticName
			, semanticIndex );
	}

	void VertexLayout::createAttributes( uint32_t count
		, uint32_t startingLocation
		, Format format
		, uint32_t startingOffset
		, std::string const & semanticName
		, uint32_t startingSemanticIndex )
	{
		for ( uint32_t i = 0u; i < count; ++i )
		{
			m_attributes.emplace_back( *this
				, format
				, startingLocation++
				, startingOffset
				, semanticName
				, startingSemanticIndex++ );
			startingOffset += getSize( format );
		}
	}
}
