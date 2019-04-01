/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DebugReportObjectType_HPP___
#define ___Ashes_DebugReportObjectType_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Opérateurs de mélange.
	*/
	enum class DebugReportObjectType
	{
		eUnknown = 0,
		eInstance = 1,
		ePhysicalDevice = 2,
		eDevice = 3,
		eQueue = 4,
		eSemaphore = 5,
		eCommandBuffer = 6,
		eFence = 7,
		eDeviceMemory = 8,
		eBuffer = 9,
		eImage = 10,
		eEvent = 11,
		eQueryPool = 12,
		eBufferView = 13,
		eImageView = 14,
		eShaderModule = 15,
		ePipelineCache = 16,
		ePipelineLayout = 17,
		eRenderPass = 18,
		ePipeline = 19,
		eDescriptorSetLayout = 20,
		eSampler = 21,
		eDescriptorPool = 22,
		eDescriptorSet = 23,
		eFramebuffer = 24,
		eCommandPool = 25,
		eSurface = 26,
		eSwapchain = 27,
		eDebugReport = 28,
		eMin,
		eMax,
	};
	/**
	*\~english
	*\brief
	*	Gets the name of the given object type.
	*\param[in] value
	*	The object type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'objet donné.
	*\param[in] value
	*	Le type d'objet.
	*\return
	*	Le nom.
	*/
	inline std::string getName( DebugReportObjectType value )
	{
		switch ( value )
		{
		case ashes::DebugReportObjectType::eUnknown:
			return "Unknown";
		case ashes::DebugReportObjectType::eInstance:
			return "Instance";
		case ashes::DebugReportObjectType::ePhysicalDevice:
			return "PhysicalDevice";
		case ashes::DebugReportObjectType::eDevice:
			return "Device";
		case ashes::DebugReportObjectType::eQueue:
			return "Queue";
		case ashes::DebugReportObjectType::eSemaphore:
			return "Semaphore";
		case ashes::DebugReportObjectType::eCommandBuffer:
			return "CommandBuffer";
		case ashes::DebugReportObjectType::eFence:
			return "Fence";
		case ashes::DebugReportObjectType::eDeviceMemory:
			return "DeviceMemory";
		case ashes::DebugReportObjectType::eBuffer:
			return "Buffer";
		case ashes::DebugReportObjectType::eImage:
			return "Image";
		case ashes::DebugReportObjectType::eEvent:
			return "Event";
		case ashes::DebugReportObjectType::eQueryPool:
			return "QueryPool";
		case ashes::DebugReportObjectType::eBufferView:
			return "BufferView";
		case ashes::DebugReportObjectType::eImageView:
			return "ImageView";
		case ashes::DebugReportObjectType::eShaderModule:
			return "ShaderModule";
		case ashes::DebugReportObjectType::ePipelineCache:
			return "PipelineCache";
		case ashes::DebugReportObjectType::ePipelineLayout:
			return "PipelineLayout";
		case ashes::DebugReportObjectType::eRenderPass:
			return "RenderPass";
		case ashes::DebugReportObjectType::ePipeline:
			return "Pipeline";
		case ashes::DebugReportObjectType::eDescriptorSetLayout:
			return "DescriptorSetLayout";
		case ashes::DebugReportObjectType::eSampler:
			return "Sampler";
		case ashes::DebugReportObjectType::eDescriptorPool:
			return "DescriptorPool";
		case ashes::DebugReportObjectType::eDescriptorSet:
			return "DescriptorSet";
		case ashes::DebugReportObjectType::eFramebuffer:
			return "Framebuffer";
		case ashes::DebugReportObjectType::eCommandPool:
			return "CommandPool";
		case ashes::DebugReportObjectType::eSurface:
			return "Surface";
		case ashes::DebugReportObjectType::eSwapchain:
			return "Swapchain";
		case ashes::DebugReportObjectType::eDebugReport:
			return "DebugReport";

		default:
			assert( false && "Unsupported DebugReportObjectType." );
			throw std::runtime_error{ "Unsupported DebugReportObjectType" };
		}

		return 0;
	}
}

#endif
