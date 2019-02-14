#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPipelineBindPoint convert( ashes::PipelineBindPoint const & point )
	{
		switch ( point )
		{
		case ashes::PipelineBindPoint::eGraphics:
			return VK_PIPELINE_BIND_POINT_GRAPHICS;

		case ashes::PipelineBindPoint::eCompute:
			return VK_PIPELINE_BIND_POINT_COMPUTE;

		default:
			assert( false && "Unsupported bind point" );
			return VkPipelineBindPoint( 0 );
		}
	}
}
