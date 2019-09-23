/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ShaderStageFlag_HPP___
#define ___AshesPP_ShaderStageFlag_HPP___
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
	inline std::string getName( VkShaderStageFlagBits value )
	{
		switch ( value )
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			return "vertex";
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return "tess_control";
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return "tess_eval";
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return "geometry";
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return "fragment";
		case VK_SHADER_STAGE_COMPUTE_BIT:
			return "compute";
#ifdef VK_NV_ray_tracing
		case VK_SHADER_STAGE_RAYGEN_BIT_NV:
			return "raygen";
		case VK_SHADER_STAGE_ANY_HIT_BIT_NV:
			return "any_hit";
		case VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV:
			return "closest_hit";
		case VK_SHADER_STAGE_MISS_BIT_NV:
			return "miss";
		case VK_SHADER_STAGE_INTERSECTION_BIT_NV:
			return "intersection";
		case VK_SHADER_STAGE_CALLABLE_BIT_NV:
			return "callable";
#endif
#ifdef VK_NV_mesh_shader
		case VK_SHADER_STAGE_TASK_BIT_NV:
			return "task";
		case VK_SHADER_STAGE_MESH_BIT_NV:
			return "mesh";
#endif
		default:
			assert( false && "Unsupported VkShaderStageFlagBits." );
			throw std::runtime_error{ "Unsupported VkShaderStageFlagBits" };
		}

		return 0;
	}
}

#endif
