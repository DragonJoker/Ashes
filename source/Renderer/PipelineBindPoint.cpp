#include "PipelineBindPoint.hpp"

namespace renderer
{
	VkPipelineBindPoint convert( PipelineBindPoint const & point )
	{
		switch ( point )
		{
		case PipelineBindPoint::eGraphics:
			return VK_PIPELINE_BIND_POINT_GRAPHICS;

		case PipelineBindPoint::eCompute:
			return VK_PIPELINE_BIND_POINT_COMPUTE;

		default:
			assert( false && "Unsupported bind point" );
			return VkPipelineBindPoint( 0 );
		}
	}
}
