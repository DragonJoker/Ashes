/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "util/UtilsDebug.hpp"

#include <ashespp/Core/Instance.hpp>

namespace utils
{
	namespace
	{
		void print( std::ostream & stream
			, std::string const & name
			, char const * value
			, std::string const & lineEnd
			, std::string const & lineBegin )
		{
			stream << lineBegin << name << ": " << ( value ? std::string( value ) : std::string() ) << lineEnd;
		}

		char const * const getName( VkDebugReportObjectTypeEXT value )
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
				return "Surface";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT:
				return "Swapchain";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT:
				return "DebugReport";

			default:
				assert( false && "Unsupported VkDebugReportObjectTypeEXT." );
				return "Undefined";
			}
		}
	}

#if VK_EXT_debug_utils
	namespace
	{
		std::ostream & operator<<( std::ostream & stream, VkDebugUtilsObjectNameInfoEXT const & value )
		{
			stream << "(" << std::hex << value.objectHandle << ") " << ashes::getName( value.objectType );

			if ( value.pObjectName )
			{
				stream << " " << value.pObjectName;
			}

			return stream;
		}

		std::ostream & operator<<( std::ostream & stream, VkDebugUtilsLabelEXT const & value )
		{
			stream << "(" << value.color[0]
				<< ", " << value.color[1]
				<< ", " << value.color[2]
				<< ", " << value.color[3] << ")";

			if ( value.pLabelName )
			{
				stream << " " << value.pLabelName;
			}

			return stream;
		}

		template< typename ObjectT >
		void print( std::ostream & stream
			, std::string const & name
			, uint32_t count
			, ObjectT const * objects
			, std::string const & lineEnd
			, std::string const & lineBegin )
		{
			stream << lineBegin << name << ": " << count << lineEnd;

			for ( uint32_t i = 0u; i < count; ++i, ++objects )
			{
				stream << lineBegin << lineBegin << *objects << lineEnd;
			}
		}

		VkBool32 VKAPI_PTR debugUtilsCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData
			, void * pUserData )
		{
			// Select prefix depending on flags passed to the callback
			// Note that multiple flags may be set for a single validation message
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "Vulkan ";
			std::string lineEnd;
			std::string lineBegin = ", ";

			// Error that may result in undefined behaviour
			switch ( messageSeverity )
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				stream << "Error";
				lineEnd = "\n";
				lineBegin = "    ";
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				stream << "Warning";
				lineEnd = "\n";
				lineBegin = "    ";
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				stream << "Info";
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				stream << "Verbose";
				break;
			default:
				break;
			}

			if ( ashes::checkFlag( messageTypes, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT ) )
			{
				stream << " - General";
			}
			if ( ashes::checkFlag( messageTypes, VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT ) )
			{
				stream << " - Validation";
			}
			if ( ashes::checkFlag( messageTypes, VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ) )
			{
				stream << " - Performance";
			}

			stream << lineEnd;
			print( stream, "Message ID", pCallbackData->pMessageIdName, lineEnd, lineBegin );
			stream << lineBegin << "Code: 0x" << std::hex << pCallbackData->messageIdNumber << lineEnd;
			print( stream, "Message", pCallbackData->pMessage, lineEnd, lineBegin );
			print( stream, "Objects", pCallbackData->objectCount, pCallbackData->pObjects, lineEnd, lineBegin );
			print( stream, "Queue Labels", pCallbackData->queueLabelCount, pCallbackData->pQueueLabels, lineEnd, lineBegin );
			print( stream, "CommmandBuffer Labels", pCallbackData->cmdBufLabelCount, pCallbackData->pCmdBufLabels, lineEnd, lineBegin );

			VkBool32 result = VK_FALSE;

			switch ( messageSeverity )
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				ashes::Logger::logError( stream );
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				ashes::Logger::logWarning( stream );
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				ashes::Logger::logTrace( stream );
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				ashes::Logger::logDebug( stream );
				break;
			default:
				break;
			}

			// The return value of this callback controls wether the Vulkan call that caused
			// the validation message will be aborted or not
			// Return VK_FALSE if we DON'T want Vulkan calls that cause a validation message 
			// (and return a VkResult) to abort
			// Return VK_TRUE if you instead want to have calls abort, and the function will 
			// return VK_ERROR_VALIDATION_FAILED_EXT 
			return result;
		}

		VkBool32 VKAPI_PTR debugUtilsCallback( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage
			, void * pUserData )
		{
			// Select prefix depending on flags passed to the callback
			// Note that multiple flags may be set for a single validation message
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
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
			print( stream, "Layer", pLayerPrefix, "\n", "    " );
			stream << "    Code: 0x" << std::hex << messageCode << "\n";
			stream << "    Object: (" << std::hex << object << ") " << getName( objectType ) << "\n";
			print( stream, "Message", pMessage, "\n", "    " );

			VkBool32 result = VK_FALSE;

#if defined( VK_USE_PLATFORM_ANDROID_KHR )

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

	VkDebugUtilsMessengerEXT setupDebugging( ashes::Instance const & instance
		, void * userData )
	{
		VkDebugUtilsMessageSeverityFlagsEXT severityFlags = 0u
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		VkDebugUtilsMessageTypeFlagsEXT typeFlags = 0u
			| VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		VkDebugUtilsMessengerCreateInfoEXT createInfo
		{
			VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			nullptr,
			0u,
			severityFlags,
			typeFlags,
			debugUtilsCallback,
			userData
		};
		return instance.createDebugUtilsMessenger( createInfo );
	}

#elif

	namespace
	{
		VkBool32 VKAPI_PTR debugReportCallback( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage
			, void * pUserData )
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
			print( stream, "Layer", pLayerPrefix, "\n", "    " );
			stream << "    Code: 0x" << std::hex << messageCode << "\n";
			stream << "    Object: (" << std::hex << object << ") " << getName( objectType ) << "\n";
			print( stream, "Message", pMessage, "\n", "    " );

			VkBool32 result = VK_FALSE;

#if defined( VK_USE_PLATFORM_ANDROID_KHR )

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

	VkDebugReportCallbackEXT setupDebugging( ashes::Instance const & instance
		, void * userData )
	{
		// The report flags determine what type of messages for the layers will be displayed
		// For validating (debugging) an appplication the error and warning bits should suffice
		VkDebugReportFlagsEXT debugReportFlags = 0u
			| VK_DEBUG_REPORT_DEBUG_BIT_EXT
			//| VK_DEBUG_REPORT_INFORMATION_BIT_EXT
			| VK_DEBUG_REPORT_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_ERROR_BIT_EXT;

		VkDebugReportCallbackCreateInfoEXT dbgCreateInfo
		{
			VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
			nullptr,
			debugReportFlags,
			debugReportCallback,
			userData
		};

		return instance.createDebugReportCallback( dbgCreateInfo );
	}
#endif
}
