/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BufferTarget_HPP___
#define ___AshesPP_BufferTarget_HPP___
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
	inline std::string getName( VkBufferUsageFlagBits value )
	{
		switch ( value )
		{
		case VK_BUFFER_USAGE_TRANSFER_SRC_BIT:
			return "transfer_src";
		case VK_BUFFER_USAGE_TRANSFER_DST_BIT:
			return "transfer_dst";
		case VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT:
			return "uniform_texel_buffer";
		case VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT:
			return "storage_texel_buffer";
		case VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT:
			return "uniform_buffer";
		case VK_BUFFER_USAGE_STORAGE_BUFFER_BIT:
			return "storage_buffer";
		case VK_BUFFER_USAGE_INDEX_BUFFER_BIT:
			return "index_buffer";
		case VK_BUFFER_USAGE_VERTEX_BUFFER_BIT:
			return "vertex_buffer";
		case VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT:
			return "indirect_buffer";
#if defined( VK_API_VERSION_1_2 )
		case VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT:
			return "shader_device_address";
#elif defined( VK_KHR_buffer_device_address )
		case VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR:
			return "shader_device_address";
#elif defined( VK_EXT_buffer_device_address )
		case VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_EXT:
			return "shader_device_address";
#endif
#ifdef VK_EXT_tranform_feedback
		case VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT:
			return "transform_feedback_buffer";
		case VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT:
			return "transform_feedback_counter_buffer";
#endif
#ifdef VK_EXT_conditional_rendering
		case VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT:
			return "conditional_rendering";
#endif
#ifdef VK_NV_ray_tracing
		case VK_BUFFER_USAGE_RAY_TRACING_BIT_NV:
			return "ray_tracing";
#endif
#ifdef VK_EXT_device_address
		case VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_EXT:
			return "shader_device_address";
#endif
		default:
			assert( false && "Unsupported VkBufferUsageFlagBits." );
			return "Unsupported VkBufferUsageFlagBits";
		}
	}
}

#endif
