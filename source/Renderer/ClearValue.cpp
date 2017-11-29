#include "ClearValue.hpp"

namespace renderer
{
	namespace
	{
		inline VkClearColorValue doConvert( utils::RgbaColour const & value )
		{
			return VkClearColorValue{ { value[0], value[1], value[2], value[3] } };
		}

		inline VkClearDepthStencilValue doConvert( DepthStencilClearValue const & value )
		{
			return VkClearDepthStencilValue{ value.depth, value.stencil };
		}
	}

	ClearValue::ClearValue( utils::RgbaColour const & colour )
		: m_colour{ colour }
		, m_isColour{ true }
	{
	}

	ClearValue::ClearValue( DepthStencilClearValue const & depthStencil )
		: m_depthStencil{ depthStencil }
	{
	}

	VkClearValue convert( ClearValue const & value )
	{
		VkClearValue result;

		if ( value.m_isColour )
		{
			result.color = doConvert( value.m_colour );
		}
		else
		{
			result.depthStencil = doConvert( value.m_depthStencil );
		}

		return result;
	}
}
