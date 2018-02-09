/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Shader/AttributeBase.hpp"

namespace renderer
{
	AttributeBase::AttributeBase( VertexLayout const & layout
		, AttributeFormat format
		, uint32_t location
		, uint32_t offset
		, uint32_t divisor )
		: m_format{ format }
		, m_location{ location }
		, m_offset{ offset }
		, m_divisor{ divisor }
	{
	}
}
