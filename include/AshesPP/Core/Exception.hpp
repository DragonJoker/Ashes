/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

#include <exception>

namespace ashespp
{
	inline std::string getName( VkResult value )
	{
		switch ( value )
		{
		case VkResult::VK_SUCCESS:
			return "Success";

		case VkResult::VK_NOT_READY:
			return "A fence or query has not yet completed.";

		case VkResult::VK_TIMEOUT:
			return "A wait operation has not completed in the specified time.";

		case VkResult::VK_EVENT_SET:
			return "An event is signaled.";

		case VkResult::VK_EVENT_RESET:
			return "An event is unsignaled.";

		case VkResult::VK_INCOMPLETE:
			return "A return array was too small for the result.";

		case VkResult::VK_ERROR_OUT_OF_HOST_MEMORY:
			return "A host memory allocation has failed.";

		case VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY:
			return "A device memory allocation has failed.";

		case VkResult::VK_ERROR_INITIALIZATION_FAILED:
			return "Initialization of an object could not be completed for implementation-specific reasons.";

		case VkResult::VK_ERROR_DEVICE_LOST:
			return "The logical or physical device has been lost.";

		case VkResult::VK_ERROR_MEMORY_MAP_FAILED:
			return "Mapping of a memory object has failed.";

		case VkResult::VK_ERROR_LAYER_NOT_PRESENT:
			return "A requested layer is not present or could not be loaded.";

		case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT:
			return "A requested extension is not supported.";

		case VkResult::VK_ERROR_FEATURE_NOT_PRESENT:
			return "A requested feature is not supported.";

		case VkResult::VK_ERROR_INCOMPATIBLE_DRIVER:
			return "The requested version of the API is not supported by the driver or is otherwise incompatible for implementation-specific reasons.";

		case VkResult::VK_ERROR_TOO_MANY_OBJECTS:
			return "Too many objects of the type have already been created.";

		case VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED:
			return "A requested format is not supported on this device.";

		case VkResult::VK_ERROR_SURFACE_LOST_KHR:
			return "A surface is no longer available.";

		case VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			return "The requested window is already connected to a surface, or to some other API.";

		case VkResult::VK_SUBOPTIMAL_KHR:
			return "An image became available, and the swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully.";

		case VkResult::VK_ERROR_OUT_OF_DATE_KHR:
			return "A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain will fail.";

		case VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			return "The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image.";

		case VkResult::VK_ERROR_VALIDATION_FAILED_EXT:
			return "Validation failed.";

		case VkResult::VK_ERROR_INVALID_SHADER_NV:
			return "One or more shaders failed to compile or link.";

		default:
			return "Unknowns error.";
		}

		return 0;
	}
	/**
	*\~english
	*\brief
	*	API exception class.
	*\~french
	*\brief
	*	Classe d'exception de l'API.
	*/
	class Exception
		: public std::exception
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] result
		*	Le résultat de l'action.
		*\param[in] text
		*	Le texte de l'exception.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] result
		*	The action result.
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
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
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

	protected:
		VkResult m_result;
		std::string m_text;
	};
}
