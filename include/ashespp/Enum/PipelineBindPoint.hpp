/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineBindPoint_HPP___
#define ___AshesPP_PipelineBindPoint_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkPipelineBindPoint value )
	{
		switch ( value )
		{
		case VK_PIPELINE_BIND_POINT_GRAPHICS:
			return "graphics";
		case VK_PIPELINE_BIND_POINT_COMPUTE:
			return "compute";
#ifdef VK_NV_ray_tracing
		case VK_PIPELINE_BIND_POINT_RAY_TRACING_NV:
			return "ray_tracing";
#endif
		default:
			assert( false && "Unsupported VkPipelineBindPoint." );
			throw std::runtime_error{ "Unsupported VkPipelineBindPoint" };
		}

		return 0;
	}
}

#endif
