/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ShaderGroupShader_HPP___
#define ___AshesPP_ShaderGroupShader_HPP___
#pragma once

namespace ashes
{
#if VK_KHR_ray_tracing_pipeline
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkShaderGroupShaderKHR value )
	{
		switch ( value )
		{
		case VK_SHADER_GROUP_SHADER_GENERAL_KHR:
			return "general";
		case VK_SHADER_GROUP_SHADER_CLOSEST_HIT_KHR:
			return "closest_hit";
		case VK_SHADER_GROUP_SHADER_ANY_HIT_KHR:
			return "any_hit";
		case VK_SHADER_GROUP_SHADER_INTERSECTION_KHR:
			return "intersection";
		default:
			assert( false && "Unsupported VkShaderGroupShaderKHR." );
			return "Unsupported VkShaderGroupShaderKHR";
		}
	}
#endif
}

#endif
