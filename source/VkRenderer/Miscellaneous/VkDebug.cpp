/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/VkInstance.hpp"

namespace vk_renderer
{
	namespace
	{
		std::string getName( VkDebugReportObjectTypeEXT value )
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
			case VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT:
				return "SurfaceKHR";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT:
				return "SwapchainKHR";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT:
				return "DebugReportCallbackEXT";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT:
				return "DisplayKHR";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT:
				return "DisplayModeKHR";
			case VK_DEBUG_REPORT_OBJECT_TYPE_OBJECT_TABLE_NVX_EXT:
				return "ObjectTableNVX";
			case VK_DEBUG_REPORT_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX_EXT:
				return "IndirectCommandsLayoutNVX";
			case VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT:
				return "ValidationCacheEXT";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_EXT:
				return "SamplerYcbcrConversion";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT:
				return "DescriptorUpdateTemplate";
			default:
				return "Unsupported";
			}
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage
			, void* pUserData )
		{
			// Select prefix depending on flags passed to the callback
			// Note that multiple flags may be set for a single validation message
			std::locale loc{ "C" };
			std::stringstream stream;
			stream.imbue( loc );
			stream << "Vulkan ";

			// Error that may result in undefined behaviour
			if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_ERROR_BIT_EXT ) )
			{
				stream << "Error:\n";
			};
			// Warnings may hint at unexpected / non-spec API usage
			if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_WARNING_BIT_EXT ) )
			{
				stream << "Warning:\n";
			};
			// May indicate sub-optimal usage of the API
			if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT ) )
			{
				stream << "Performance:\n";
			};
			// Informal messages that may become handy during debugging
			if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_INFORMATION_BIT_EXT ) )
			{
				stream << "Info:\n";
			}
			// Diagnostic info from the Vulkan loader and layers
			// Usually not helpful in terms of API usage, but may help to debug layer and loader problems 
			if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_DEBUG_BIT_EXT ) )
			{
				stream << "Debug:\n";
			}

			// Display message to default output (console/logcat)
			stream << "    Layer: " << pLayerPrefix << "\n";
			stream << "    Code: 0x" << std::hex << messageCode << "\n";
			stream << "    Object: (" << std::hex << object << ") " << getName( objectType ) << "\n";
			stream << "    Message: " << pMessage;

			VkBool32 result = VK_FALSE;

#if defined( __ANDROID__ )

			if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_ERROR_BIT_EXT ) )
			{
				LOGE( "%s", stream.str().c_str() );
			}
			else
			{
				LOGD( "%s", stream.str().c_str() );
			}

			fflush( stdout );

#else

			if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_ERROR_BIT_EXT ) )
			{
				ashes::Logger::logError( stream );
			}
			else if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_WARNING_BIT_EXT )
				|| ashes::checkFlag( flags, VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT ) )
			{
				ashes::Logger::logWarning( stream );
			}
			else if ( ashes::checkFlag( flags, VK_DEBUG_REPORT_INFORMATION_BIT_EXT ) )
			{
				ashes::Logger::logTrace( stream );
			}
			else
			{
				ashes::Logger::logDebug( stream );
			}

#endif

			// The return value of this callback controls wether the Vulkan call that caused
			// the validation message will be aborted or not
			// Return VK_FALSE if we DON'T want Vulkan calls that cause a validation message 
			// (and return a VkResult) to abort
			// Return VK_TRUE if you instead want to have calls abort, and the function will 
			// return VK_ERROR_VALIDATION_FAILED_EXT 
			return result;
		}

	}

	std::multimap< std::string, std::string > Debug::m_dump;

	template<> std::string const VkTypeNamer< VkApplicationInfo >::Name = "VkApplicationInfo";
	template<> std::string const VkTypeNamer< VkAttachmentDescription >::Name = "VkAttachmentDescription";
	template<> std::string const VkTypeNamer< VkAttachmentReference >::Name = "VkAttachmentReference";
	template<> std::string const VkTypeNamer< VkBufferCopy >::Name = "VkBufferCopy";
	template<> std::string const VkTypeNamer< VkBufferCreateInfo >::Name = "VkBufferCreateInfo";
	template<> std::string const VkTypeNamer< VkBufferImageCopy >::Name = "VkBufferImageCopy";
	template<> std::string const VkTypeNamer< VkBufferMemoryBarrier >::Name = "VkBufferMemoryBarrier";
	template<> std::string const VkTypeNamer< VkBufferViewCreateInfo >::Name = "VkBufferViewCreateInfo";
	template<> std::string const VkTypeNamer< VkClearColorValue >::Name = "VkClearColorValue";
	template<> std::string const VkTypeNamer< VkClearDepthStencilValue >::Name = "VkClearDepthStencilValue";
	template<> std::string const VkTypeNamer< VkClearValue >::Name = "VkClearValue";
	template<> std::string const VkTypeNamer< VkCommandBufferAllocateInfo >::Name = "VkCommandBufferAllocateInfo";
	template<> std::string const VkTypeNamer< VkCommandBufferBeginInfo >::Name = "VkCommandBufferBeginInfo";
	template<> std::string const VkTypeNamer< VkCommandBufferInheritanceInfo >::Name = "VkCommandBufferInheritanceInfo";
	template<> std::string const VkTypeNamer< VkCommandPoolCreateInfo >::Name = "VkCommandPoolCreateInfo";
	template<> std::string const VkTypeNamer< VkComponentMapping >::Name = "VkComponentMapping";
	template<> std::string const VkTypeNamer< VkComputePipelineCreateInfo >::Name = "VkComputePipelineCreateInfo";
	template<> std::string const VkTypeNamer< VkDescriptorBufferInfo >::Name = "VkDescriptorBufferInfo";
	template<> std::string const VkTypeNamer< VkDescriptorImageInfo >::Name = "VkDescriptorImageInfo";
	template<> std::string const VkTypeNamer< VkDescriptorPoolCreateInfo >::Name = "VkDescriptorPoolCreateInfo";
	template<> std::string const VkTypeNamer< VkDescriptorPoolSize >::Name = "VkDescriptorPoolSize";
	template<> std::string const VkTypeNamer< VkDescriptorSetAllocateInfo >::Name = "VkDescriptorSetAllocateInfo";
	template<> std::string const VkTypeNamer< VkDescriptorSetLayoutBinding >::Name = "VkDescriptorSetLayoutBinding";
	template<> std::string const VkTypeNamer< VkDescriptorSetLayoutCreateInfo >::Name = "VkDescriptorSetLayoutCreateInfo";
	template<> std::string const VkTypeNamer< VkDeviceCreateInfo >::Name = "VkDeviceCreateInfo";
	template<> std::string const VkTypeNamer< VkDeviceQueueCreateInfo >::Name = "VkDeviceQueueCreateInfo";
	template<> std::string const VkTypeNamer< VkExtensionProperties >::Name = "VkExtensionProperties";
	template<> std::string const VkTypeNamer< VkExtent2D >::Name = "VkExtent2D";
	template<> std::string const VkTypeNamer< VkExtent3D >::Name = "VkExtent3D";
	template<> std::string const VkTypeNamer< VkFenceCreateInfo >::Name = "VkFenceCreateInfo";
	template<> std::string const VkTypeNamer< VkFramebufferCreateInfo >::Name = "VkFramebufferCreateInfo";
	template<> std::string const VkTypeNamer< VkGraphicsPipelineCreateInfo >::Name = "VkGraphicsPipelineCreateInfo";
	template<> std::string const VkTypeNamer< VkImageBlit >::Name = "VkImageBlit";
	template<> std::string const VkTypeNamer< VkImageCopy >::Name = "VkImageCopy";
	template<> std::string const VkTypeNamer< VkImageCreateInfo >::Name = "VkImageCreateInfo";
	template<> std::string const VkTypeNamer< VkImageMemoryBarrier >::Name = "VkImageMemoryBarrier";
	template<> std::string const VkTypeNamer< VkImageSubresourceLayers >::Name = "VkImageSubresourceLayers";
	template<> std::string const VkTypeNamer< VkImageSubresourceRange >::Name = "VkImageSubresourceRange";
	template<> std::string const VkTypeNamer< VkImageViewCreateInfo >::Name = "VkImageViewCreateInfo";
	template<> std::string const VkTypeNamer< VkInstanceCreateInfo >::Name = "VkInstanceCreateInfo";
	template<> std::string const VkTypeNamer< VkLayerProperties >::Name = "VkLayerProperties";
	template<> std::string const VkTypeNamer< VkMappedMemoryRange >::Name = "VkMappedMemoryRange";
	template<> std::string const VkTypeNamer< VkMemoryAllocateInfo >::Name = "VkMemoryAllocateInfo";
	template<> std::string const VkTypeNamer< VkOffset2D >::Name = "VkOffset2D";
	template<> std::string const VkTypeNamer< VkOffset3D >::Name = "VkOffset3D";
	template<> std::string const VkTypeNamer< VkPhysicalDeviceFeatures >::Name = "VkPhysicalDeviceFeatures";
	template<> std::string const VkTypeNamer< VkPipelineColorBlendAttachmentState >::Name = "VkPipelineColorBlendAttachmentState";
	template<> std::string const VkTypeNamer< VkPipelineColorBlendStateCreateInfo >::Name = "VkPipelineColorBlendStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineDepthStencilStateCreateInfo >::Name = "VkPipelineDepthStencilStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineDynamicStateEnableCreateInfo >::Name = "VkPipelineDynamicStateEnableCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineInputAssemblyStateCreateInfo >::Name = "VkPipelineInputAssemblyStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineLayoutCreateInfo >::Name = "VkPipelineLayoutCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineMultisampleStateCreateInfo >::Name = "VkPipelineMultisampleStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineRasterizationStateCreateInfo >::Name = "VkPipelineRasterizationStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineShaderStageCreateInfo >::Name = "VkPipelineShaderStageCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineTessellationStateCreateInfo >::Name = "VkPipelineTessellationStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineVertexInputStateCreateInfo >::Name = "VkPipelineVertexInputStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPipelineViewportStateCreateInfo >::Name = "VkPipelineViewportStateCreateInfo";
	template<> std::string const VkTypeNamer< VkPresentInfoKHR >::Name = "VkPresentInfoKHR";
	template<> std::string const VkTypeNamer< VkPushConstantRange >::Name = "VkPushConstantRange";
	template<> std::string const VkTypeNamer< VkQueryPoolCreateInfo >::Name = "VkQueryPoolCreateInfo";
	template<> std::string const VkTypeNamer< VkRect2D >::Name = "VkRect2D";
	template<> std::string const VkTypeNamer< VkRenderPassBeginInfo >::Name = "VkRenderPassBeginInfo";
	template<> std::string const VkTypeNamer< VkRenderPassCreateInfo >::Name = "VkRenderPassCreateInfo";
	template<> std::string const VkTypeNamer< VkSamplerCreateInfo >::Name = "VkSamplerCreateInfo";
	template<> std::string const VkTypeNamer< VkSemaphoreCreateInfo >::Name = "VkSemaphoreCreateInfo";
	template<> std::string const VkTypeNamer< VkShaderModuleCreateInfo >::Name = "VkShaderModuleCreateInfo";
	template<> std::string const VkTypeNamer< VkSpecializationInfo >::Name = "VkSpecializationInfo";
	template<> std::string const VkTypeNamer< VkSpecializationMapEntry >::Name = "VkSpecializationMapEntry";
	template<> std::string const VkTypeNamer< VkStencilOpState >::Name = "VkStencilOpState";
	template<> std::string const VkTypeNamer< VkSubmitInfo >::Name = "VkSubmitInfo";
	template<> std::string const VkTypeNamer< VkSubpassDependency >::Name = "VkSubpassDependency";
	template<> std::string const VkTypeNamer< VkSubpassDescription >::Name = "VkSubpassDescription";
	template<> std::string const VkTypeNamer< VkSwapchainCreateInfoKHR >::Name = "VkSwapchainCreateInfoKHR";
	template<> std::string const VkTypeNamer< VkVertexInputAttributeDescription >::Name = "VkVertexInputAttributeDescription";
	template<> std::string const VkTypeNamer< VkVertexInputBindingDescription >::Name = "VkVertexInputBindingDescription";
	template<> std::string const VkTypeNamer< VkViewport >::Name = "VkViewport";
	template<> std::string const VkTypeNamer< VkWriteDescriptorSet >::Name = "VkWriteDescriptorSet";

#ifdef VK_USE_PLATFORM_WIN32_KHR

	template<> std::string const VkTypeNamer< VkWin32SurfaceCreateInfoKHR >::Name = "VkWin32SurfaceCreateInfoKHR";

#endif
	void addOptionalValidationLayer( std::string const & layerName
		, std::string description
		, ashes::StringArray & names )
	{
#if LOAD_VALIDATION_LAYERS
		if ( layerName.find( "validation" ) != std::string::npos
			|| description.find( "LunarG Validation" ) != std::string::npos )
		{
			names.push_back( layerName );
		}
#endif
	}

	void addOptionalDebugReportLayer( ashes::StringArray & names )
	{
#if LOAD_VALIDATION_LAYERS
		names.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
		//names.push_back( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
#endif
	}

	void setupDebugging( VkInstance instance
		, Instance & data
		, VkDebugReportCallbackEXT & callback )
	{
#if LOAD_VALIDATION_LAYERS
		// The report flags determine what type of messages for the layers will be displayed
		// For validating (debugging) an appplication the error and warning bits should suffice
		VkDebugReportFlagsEXT debugReportFlags = /*VK_DEBUG_REPORT_INFORMATION_BIT_EXT
			| */VK_DEBUG_REPORT_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_ERROR_BIT_EXT
			| VK_DEBUG_REPORT_DEBUG_BIT_EXT;

		if ( data.vkCreateDebugReportCallbackEXT )
		{
			VkDebugReportCallbackCreateInfoEXT dbgCreateInfo
			{
				VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
				nullptr,
				debugReportFlags,
				( PFN_vkDebugReportCallbackEXT )debugReportCallback,
				&data
			};

			VkResult res = data.vkCreateDebugReportCallbackEXT(
				instance,
				&dbgCreateInfo,
				nullptr,
				&callback );
			checkError( res, "Debug initialisation" );
		}
#endif
	}

	void cleanupDebugging( VkInstance instance
		, Instance & data
		, VkDebugReportCallbackEXT & callback )
	{
#if LOAD_VALIDATION_LAYERS
		if ( callback != VK_NULL_HANDLE )
		{
			data.vkDestroyDebugReportCallbackEXT( instance, callback, nullptr );
			callback = VK_NULL_HANDLE;
		}
#endif
	}
}
