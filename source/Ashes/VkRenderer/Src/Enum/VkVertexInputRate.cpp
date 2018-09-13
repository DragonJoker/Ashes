#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkVertexInputRate convert( ashes::VertexInputRate const & value )
	{
		switch ( value )
		{
		case ashes::VertexInputRate::eVertex:
			return VK_VERTEX_INPUT_RATE_VERTEX;

		case ashes::VertexInputRate::eInstance:
			return VK_VERTEX_INPUT_RATE_INSTANCE;

		default:
			assert( false );
			return VK_VERTEX_INPUT_RATE_VERTEX;
		}
	}
}
