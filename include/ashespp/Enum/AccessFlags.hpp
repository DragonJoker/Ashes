/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AccessFlag_HPP___
#define ___AshesPP_AccessFlag_HPP___
#pragma once

#include <stdexcept>

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
	inline std::string getName( VkAccessFlagBits value )
	{
		switch ( value )
		{
		case VK_ACCESS_INDIRECT_COMMAND_READ_BIT:
			return "indirect_command_read";
		case VK_ACCESS_INDEX_READ_BIT:
			return "index_read";
		case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT:
			return "vertex_attribute_read";
		case VK_ACCESS_UNIFORM_READ_BIT:
			return "uniform_read";
		case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT:
			return "input_attachment_read";
		case VK_ACCESS_SHADER_READ_BIT:
			return "shader_read";
		case VK_ACCESS_SHADER_WRITE_BIT:
			return "shader_write";
		case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT:
			return "color_attachment_read";
		case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT:
			return "color_attachment_write";
		case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT:
			return "depth_stencil_attachment_read";
		case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT:
			return "depth_stencil_attachment_write";
		case VK_ACCESS_TRANSFER_READ_BIT:
			return "transfer_read";
		case VK_ACCESS_TRANSFER_WRITE_BIT:
			return "transfer_write";
		case VK_ACCESS_HOST_READ_BIT:
			return "host_read";
		case VK_ACCESS_HOST_WRITE_BIT:
			return "host_write";
		case VK_ACCESS_MEMORY_READ_BIT:
			return "memory_read";
		case VK_ACCESS_MEMORY_WRITE_BIT:
			return "memory_write";
#ifdef VK_EXT_transform_feedback
		case VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT:
			return "transform_feedback_write";
		case VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT:
			return "transform_feedback_counter_read";
		case VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT:
			return "transform_feedback_counter_write";
#endif
#ifdef VK_EXT_conditional_rendering
		case VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT:
			return "conditional_rendering_read";
#endif
#ifdef VK_NVX_device_generated_commands
		case VK_ACCESS_COMMAND_PROCESS_READ_BIT_NVX:
			return "command_process_read";
		case VK_ACCESS_COMMAND_PROCESS_WRITE_BIT_NVX:
			return "command_process_write";
#endif
#ifdef VK_EXT_blend_operation_advanced
		case VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT:
			return "color_attachment_read_noncoherent";
#endif
#ifdef VK_NV_shading_rate_image
		case VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV:
			return "shading_rate_image_read";
#endif
#ifdef VK_NV_ray_tracing
		case VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV:
			return "acceleration_structure_read";
		case VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV:
			return "acceleration_structure_write";
#endif
#ifdef VK_EXT_fragment_density_map
		case VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT:
			return "fragment_density_map_read";
#endif
		default:
			assert( false && "Unsupported VkAccessFlagBits." );
			return "Unsupported VkAccessFlagBits";
		}
	}
}

#endif
