/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineStageFlags_HPP___
#define ___AshesPP_PipelineStageFlags_HPP___
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
	inline std::string getName( VkPipelineStageFlagBits value )
	{
		switch ( value )
		{
		case VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT:
			return "top_of_pipe";
		case VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT:
			return "draw_indirect";
		case VK_PIPELINE_STAGE_VERTEX_INPUT_BIT:
			return "vertex_input";
		case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
			return "vertex_shader";
		case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
			return "tessellation_control_shader";
		case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
			return "tessellation_evaluation_shader";
		case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
			return "geometry_shader";
		case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
			return "fragment_shader";
		case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
			return "early_fragment_tests";
		case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
			return "late_fragment_tests";
		case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
			return "color_attachment_output";
		case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
			return "compute_shader";
		case VK_PIPELINE_STAGE_TRANSFER_BIT:
			return "transfer";
		case VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT:
			return "bottom_of_pipe";
		case VK_PIPELINE_STAGE_HOST_BIT:
			return "host";
		case VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT:
			return "all_graphics";
		case VK_PIPELINE_STAGE_ALL_COMMANDS_BIT:
			return "all_commands";
#ifdef VK_EXT_transform_feedback
		case VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT:
			return "transform_feedback";
#endif
#ifdef VK_EXT_conditional_rendering
		case VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT:
			return "conditional_rendering";
#endif
#ifdef VK_NVX_device_generated_commands
		case VK_PIPELINE_STAGE_COMMAND_PROCESS_BIT_NVX:
			return "command_process";
#endif
#ifdef VK_NV_shading_rate_image
		case VK_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV:
			return "shading_rate_image";
#endif
#ifdef VK_NV_ray_tracing
		case VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV:
			return "ray_tracing_shader";
		case VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV:
			return "acceleration_structure_build";
#endif
#ifdef VK_NV_mesh_shader
		case VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV:
			return "task_shader";
		case VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV:
			return "mesh_shader";
#endif
#ifdef VK_EXT_fragment_density_map
		case VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT:
			return "fragment_density_process";
#endif
		default:
			assert( false && "Unsupported VkPipelineStageFlagBits." );
			throw std::runtime_error{ "Unsupported VkPipelineStageFlagBits" };
		}

		return 0;
	}
}

#endif
