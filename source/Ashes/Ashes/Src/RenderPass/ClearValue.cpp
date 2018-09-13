/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/ClearValue.hpp"

namespace ashes
{
	ClearValue::ClearValue()
		: m_colour{}
		, m_isColour{ true }
	{
	}

	ClearValue::ClearValue( ClearColorValue const & colour )
		: m_colour{ colour }
		, m_isColour{ true }
	{
	}

	ClearValue::ClearValue( DepthStencilClearValue const & depthStencil )
		: m_depthStencil{ depthStencil }
	{
	}
}
