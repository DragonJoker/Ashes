/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ObjectType_HPP___
#define ___AshesPP_ObjectType_HPP___
#pragma once

namespace ashes
{
#ifdef VK_EXT_debug_utils

	/**
	*\brief
	*	Gets the name of the given object type.
	*\param[in] value
	*	The object type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkObjectType value )
	{
		switch ( value )
		{
		case VK_OBJECT_TYPE_UNKNOWN:
			return "Unknown";
		case VK_OBJECT_TYPE_INSTANCE:
			return "Instance";
		case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
			return "PhysicalDevice";
		case VK_OBJECT_TYPE_DEVICE:
			return "Device";
		case VK_OBJECT_TYPE_QUEUE:
			return "Queue";
		case VK_OBJECT_TYPE_SEMAPHORE:
			return "Semaphore";
		case VK_OBJECT_TYPE_COMMAND_BUFFER:
			return "CommandBuffer";
		case VK_OBJECT_TYPE_FENCE:
			return "Fence";
		case VK_OBJECT_TYPE_DEVICE_MEMORY:
			return "DeviceMemory";
		case VK_OBJECT_TYPE_BUFFER:
			return "Buffer";
		case VK_OBJECT_TYPE_IMAGE:
			return "Image";
		case VK_OBJECT_TYPE_EVENT:
			return "Event";
		case VK_OBJECT_TYPE_QUERY_POOL:
			return "QueryPool";
		case VK_OBJECT_TYPE_BUFFER_VIEW:
			return "BufferView";
		case VK_OBJECT_TYPE_IMAGE_VIEW:
			return "ImageView";
		case VK_OBJECT_TYPE_SHADER_MODULE:
			return "ShaderModule";
		case VK_OBJECT_TYPE_PIPELINE_CACHE:
			return "PipelineCache";
		case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
			return "PipelineLayout";
		case VK_OBJECT_TYPE_RENDER_PASS:
			return "RenderPass";
		case VK_OBJECT_TYPE_PIPELINE:
			return "Pipeline";
		case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
			return "DescriptorSetLayout";
		case VK_OBJECT_TYPE_SAMPLER:
			return "Sampler";
		case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
			return "DescriptorPool";
		case VK_OBJECT_TYPE_DESCRIPTOR_SET:
			return "DescriptorSet";
		case VK_OBJECT_TYPE_FRAMEBUFFER:
			return "Framebuffer";
		case VK_OBJECT_TYPE_COMMAND_POOL:
			return "CommandPool";
#ifdef VK_API_VERSION_1_1
		case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:
			return "SamplerYcbcrConversion";
		case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:
			return "DescriptorUpdateTemplate";
#endif
#ifdef VK_KHR_surface
		case VK_OBJECT_TYPE_SURFACE_KHR:
			return "Surface";
#endif
#ifdef VK_KHR_swapchain
		case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
			return "Swapchain";
#endif
#ifdef VK_EXT_display
		case VK_OBJECT_TYPE_DISPLAY_KHR:
			return "Display";
		case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
			return "DisplayMode";
#endif
#ifdef VK_EXT_debug_report
		case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:
			return "DebugReportCallback";
#endif
#ifdef VK_NV_device_generated_commands
		case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV:
			return "IndirectCommandsLayout";
#endif
#ifdef VK_NVX_device_generated_commands
		case VK_OBJECT_TYPE_OBJECT_TABLE_NVX:
			return "ObjectTable";
		case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX:
			return "IndirectCommandsLayout";
#endif
#ifdef VK_EXT_debug_utils
		case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:
			return "DebugUtilsMessenger";
#endif
#ifdef VK_EXT_validation_cache
		case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:
			return "ValidationCache";
#endif
#ifdef VK_NV_ray_tracing
		case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:
			return "AccelerationStructure";
#endif
		default:
			assert( false && "Unsupported VkDebugReportObjectTypeEXT." );
			throw std::runtime_error{ "Unsupported VkDebugReportObjectTypeEXT" };
		}

		return 0;
	}

#endif
}

#endif
