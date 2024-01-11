/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Ashes_common_Exception_HPP___
#define ___Ashes_common_Exception_HPP___
#pragma once

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>

namespace ashes
{
	inline std::string getName( VkResult value )
	{
		switch ( value )
		{
		case VK_SUCCESS:
			return "Success";
		case VK_NOT_READY:
			return "A fence or query has not yet completed.";
		case VK_TIMEOUT:
			return "A wait operation has not completed in the specified time.";
		case VK_EVENT_SET:
			return "An event is signaled.";
		case VK_EVENT_RESET:
			return "An event is unsignaled.";
		case VK_INCOMPLETE:
			return "A return array was too small for the result.";
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			return "A host memory allocation has failed.";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			return "A device memory allocation has failed.";
		case VK_ERROR_INITIALIZATION_FAILED:
			return "Initialization of an object could not be completed for implementation-specific reasons.";
		case VK_ERROR_DEVICE_LOST:
			return "The logical or physical device has been lost.";
		case VK_ERROR_MEMORY_MAP_FAILED:
			return "Mapping of a memory object has failed.";
		case VK_ERROR_LAYER_NOT_PRESENT:
			return "A requested layer is not present or could not be loaded.";
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			return "A requested extension is not supported.";
		case VK_ERROR_FEATURE_NOT_PRESENT:
			return "A requested feature is not supported.";
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			return "The requested version of the API is not supported by the driver or is otherwise incompatible for implementation-specific reasons.";
		case VK_ERROR_TOO_MANY_OBJECTS:
			return "Too many objects of the type have already been created.";
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			return "A requested format is not supported on this device.";
		case VK_ERROR_FRAGMENTED_POOL:
			return "A pool allocation has failed due to fragmentation of the pool’s memory.";
		case VK_ERROR_UNKNOWN:
			return "An unknown error has occurred; either the application has provided invalid input, or an implementation failure has occurred.";
#if VK_VERSION_1_1
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			return "A pool memory allocation has failed.";
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:
			return "An external handle is not a valid handle of the specified type.";
#endif
#if VK_VERSION_1_2
		case VK_ERROR_FRAGMENTATION:
			return "A descriptor pool creation has failed due to fragmentation.";
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
			return "A buffer creation or memory allocation failed because the requested address is not available. A shader group handle assignment failed because the requested shader group handle information is no longer valid.";
#endif
#if VK_VERSION_1_3
		case VK_PIPELINE_COMPILE_REQUIRED:
			return "A requested pipeline creation would have required compilation, but the application requested compilation to not be performed.";
#endif
#if VK_KHR_surface
		case VK_ERROR_SURFACE_LOST_KHR:
			return "A surface is no longer available.";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			return "The requested window is already connected to a surface, or to some other API.";
#endif
#if VK_KHR_swapchain
		case VK_SUBOPTIMAL_KHR:
			return "An image became available, and the swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully.";
		case VK_ERROR_OUT_OF_DATE_KHR:
			return "A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain will fail.";
#endif
#if VK_KHR_display_swapchain
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			return "The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image.";
#endif
#if VK_EXT_debug_report
		case VK_ERROR_VALIDATION_FAILED_EXT:
			return "Validation failed.";
#endif
		case VK_ERROR_INVALID_SHADER_NV:
			return "One or more shaders failed to compile or link.";
#ifdef VK_ENABLE_BETA_EXTENSIONS
#	if VK_KHR_video_queue
		case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
			return "The requested VkImageUsageFlags are not supported.";
		case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
			return "The requested video picture layout is not supported.";
		case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
			return "A video profile operation specified via VkVideoProfileInfoKHR::videoCodecOperation is not supported.";
		case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
			return "Format parameters in a requested VkVideoProfileInfoKHR chain are not supported.";
		case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
			return "Codec-specific parameters in a requested VkVideoProfileInfoKHR chain are not supported.";
		case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
			return "The specified video Std header version is not supported.";
#	endif
#endif
#if VK_EXT_image_drm_format_modifier
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
			return "";
#endif
#if VK_KHR_global_priority
		case VK_ERROR_NOT_PERMITTED_KHR:
			return "Not permitted.";
#endif
#if VK_EXT_full_screen_exclusive
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
			return "An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as it did not have exclusive full-screen access.";
#endif
#if VK_KHR_deferred_host_operations
		case VK_THREAD_IDLE_KHR:
			return "A deferred operation is not complete but there is currently no work for this thread to do at the time of this call.";
		case VK_THREAD_DONE_KHR:
			return "A deferred operation is not complete but there is no work remaining to assign to additional threads.";
		case VK_OPERATION_DEFERRED_KHR:
			return "A deferred operation was requested and at least some of the work was deferred.";
		case VK_OPERATION_NOT_DEFERRED_KHR:
			return "A deferred operation was requested and no operations were deferred.";
#endif
#if VK_EXT_image_compression_control
		case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
			return "One or more shaders failed to compile or link.";
#endif
#if VK_EXT_shader_object
		case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
			return "One or more shaders failed to compile or link.";
#endif
		default:
			return "Unknown error.";
		}
	}
	/**
	*\brief
	*	API exception class.
	*/
	class Exception
		: public std::exception
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] result
		*	The Vullkan result.
		*\param[in] text
		*	The exception text.
		*/
		Exception( VkResult result
			, std::string text )
			: m_result{ result }
			, m_text{ std::move( text ) }
		{
			m_text += " - " + getName( result );
		}
		/**
		*name
		*	Getters.
		*/
		/**@{*/
		inline VkResult getResult()const
		{
			return m_result;
		}

		inline char const * what()const noexcept override
		{
			return m_text.c_str();
		}
		/**@}*/

	private:
		VkResult m_result;
		std::string m_text;
	};
	/**
	*\brief
	*	API exception class.
	*/
	class BaseException
		: public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};
}

#endif
