/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineCreateFlags_HPP___
#define ___AshesPP_PipelineCreateFlags_HPP___
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
	inline std::string getName( VkPipelineCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT:
			return "create_disable_optimization_bit";
		case VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT:
			return "create_allow_derivatives_bit";
		case VK_PIPELINE_CREATE_DERIVATIVE_BIT:
			return "create_derivative_bit";
#if VK_VERSION_1_1
		case VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT:
			return "create_view_index_from_device_index_bit";
		case VK_PIPELINE_CREATE_DISPATCH_BASE_BIT:
			return "create_dispatch_base_bit";
#endif
#if VK_KHR_fragment_shading_rate && VK_KHR_dynamic_rendering
		case VK_PIPELINE_RASTERIZATION_STATE_CREATE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR:
			return "rasterization_state_create_fragment_shading_rate_attachment_bit_khr";
#endif
#if VK_KHR_fragment_shading_rate && VK_EXT_fragment_density_map
		case VK_PIPELINE_RASTERIZATION_STATE_CREATE_FRAGMENT_DENSITY_MAP_ATTACHMENT_BIT_EXT:
			return "rasterization_state_create_fragment_density_map_attachment_bit_ext";
#endif
#if VK_KHR_ray_tracing_pipeline
		case VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_ANY_HIT_SHADERS_BIT_KHR:
			return "create_ray_tracing_no_null_any_hit_shaders_bit_khr";
		case VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_CLOSEST_HIT_SHADERS_BIT_KHR:
			return "create_ray_tracing_no_null_closest_hit_shaders_bit_khr";
		case VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_MISS_SHADERS_BIT_KHR:
			return "create_ray_tracing_no_null_miss_shaders_bit_khr";
		case VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_INTERSECTION_SHADERS_BIT_KHR:
			return "create_ray_tracing_no_null_intersection_shaders_bit_khr";
		case VK_PIPELINE_CREATE_RAY_TRACING_SKIP_TRIANGLES_BIT_KHR:
			return "create_ray_tracing_skip_triangles_bit_khr";
		case VK_PIPELINE_CREATE_RAY_TRACING_SKIP_AABBS_BIT_KHR:
			return "create_ray_tracing_skip_aabbs_bit_khr";
		case VK_PIPELINE_CREATE_RAY_TRACING_SHADER_GROUP_HANDLE_CAPTURE_REPLAY_BIT_KHR:
			return "create_ray_tracing_shader_group_handle_capture_replay_bit_khr";
#endif
#if VK_NV_ray_tracing
		case VK_PIPELINE_CREATE_DEFER_COMPILE_BIT_NV:
			return "create_defer_compile_bit_nv";
#endif
#if VK_KHR_pipeline_executable_properties
		case VK_PIPELINE_CREATE_CAPTURE_STATISTICS_BIT_KHR:
			return "create_capture_statistics_bit_khr";
		case VK_PIPELINE_CREATE_CAPTURE_INTERNAL_REPRESENTATIONS_BIT_KHR:
			return "create_capture_internal_representations_bit_khr";
#endif
#if VK_NV_device_generated_commands
		case VK_PIPELINE_CREATE_INDIRECT_BINDABLE_BIT_NV:
			return "create_indirect_bindable_bit_nv";
#endif
#if VK_KHR_pipeline_library
		case VK_PIPELINE_CREATE_LIBRARY_BIT_KHR:
			return "create_library_bit_khr";
#endif
#if VK_EXT_pipeline_creation_cache_control || VK_VERSION_1_3
		case VK_PIPELINE_CREATE_FAIL_ON_PIPELINE_COMPILE_REQUIRED_BIT_EXT:
			return "create_fail_on_pipeline_compile_required_bit_ext";
		case VK_PIPELINE_CREATE_EARLY_RETURN_ON_FAILURE_BIT_EXT:
			return "create_early_return_on_failure_bit_ext";
#endif
#if VK_NV_ray_tracing_motion_blur
		case VK_PIPELINE_CREATE_RAY_TRACING_ALLOW_MOTION_BIT_NV:
			return "create_ray_tracing_allow_motion_bit_nv";
#endif
		default:
			assert( false && "Unsupported VkPipelineCreateFlagBits." );
			return "Unsupported VkPipelineCreateFlagBits";
		}
	}
}

#endif
