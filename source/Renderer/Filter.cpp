#include "Filter.hpp"

namespace renderer
{
	VkFilter convert( Filter const & filter )
	{
		switch ( filter )
		{
		case Filter::eNearest:
			return VK_FILTER_NEAREST;

		case Filter::eLinear:
			return VK_FILTER_LINEAR;

		default:
			assert( false );
			return VK_FILTER_NEAREST;
		}
	}

}
