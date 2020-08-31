/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DebugReportObjectType_HPP___
#define ___AshesPP_DebugReportObjectType_HPP___
#pragma once

namespace ashes
{
#if VK_EXT_debug_report

	/**
	*\brief
	*	Gets the name of the given object type.
	*\param[in] value
	*	The object type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkDebugReportObjectTypeEXT value )
	{
		switch ( value )
		{
		case VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT:
			return "Unknown";
		case VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT:
			return "Instance";
		case VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT:
			return "PhysicalDevice";
		case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT:
			return "Device";
		case VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT:
			return "Queue";
		case VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT:
			return "Semaphore";
		case VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT:
			return "CommandBuffer";
		case VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT:
			return "Fence";
		case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT:
			return "DeviceMemory";
		case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT:
			return "Buffer";
		case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT:
			return "Image";
		case VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT:
			return "Event";
		case VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT:
			return "QueryPool";
		case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT:
			return "BufferView";
		case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT:
			return "ImageView";
		case VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT:
			return "ShaderModule";
		case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT:
			return "PipelineCache";
		case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT:
			return "PipelineLayout";
		case VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT:
			return "RenderPass";
		case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT:
			return "Pipeline";
		case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT:
			return "DescriptorSetLayout";
		case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT:
			return "Sampler";
		case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT:
			return "DescriptorPool";
		case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT:
			return "DescriptorSet";
		case VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT:
			return "Framebuffer";
		case VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT:
			return "CommandPool";
#	ifdef VK_KHR_surface
		case VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT:
			return "Surface";
#	endif
#	ifdef VK_KHR_swapchain
		case VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT:
			return "Swapchain";
#	endif
#	if VK_EXT_DEBUG_REPORT_SPEC_VERSION >= 8
		case VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT:
			return "DebugReport";
#	else
		case VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT:
			return "DebugReport";
#	endif
#	ifdef VK_EXT_display
		case VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT:
			return "Display";
		case VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT:
			return "DisplayMode";
#	endif
#	ifdef VK_NVX_device_generated_commands
		case VK_DEBUG_REPORT_OBJECT_TYPE_OBJECT_TABLE_NVX_EXT:
			return "ObjectTable";
		case VK_DEBUG_REPORT_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX_EXT:
			return "IndirectCommandsLayout";
#	endif
#	ifdef VK_EXT_validation_cache
		case VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT:
			return "ValidationCache";
#	endif
#	ifdef VK_VERSION_1_1
		case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_EXT:
			return "SamplerYcbcrConversion";
		case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT:
			return "DescriptorUpdateTemplate";
#	endif
#	ifdef VK_NV_ray_tracing
		case VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV_EXT:
			return "AccelerationStructure";
#	endif
		default:
			assert( false && "Unsupported VkDebugReportObjectTypeEXT." );
			throw std::runtime_error{ "Unsupported VkDebugReportObjectTypeEXT" };
		}

		return 0;
	}

#endif
}

#endif
