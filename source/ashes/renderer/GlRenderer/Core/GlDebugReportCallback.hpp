/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
#if VK_EXT_debug_utils

	class DebugUtilsMessengerEXT
	{
	public:
		DebugUtilsMessengerEXT( VkAllocationCallbacks const * callbacks
			, VkInstance instance
			, VkDebugUtilsMessengerCreateInfoEXT createInfo );
		void submit( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData
			, void * userData )const noexcept;

		void report( GlDebugSource source
			, GlDebugType type
			, uint32_t id
			, GlDebugSeverity severity
			, int length
			, const char * const message )const noexcept;
		void report( uint32_t id
			, GlDebugCategory category
			, GlDebugSeverity severity
			, int length
			, const char * const message )const noexcept;

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerCreateInfoEXT m_createInfo;
	};

#endif
#if VK_EXT_debug_report

	class DebugReportCallbackEXT
	{
	public:
		DebugReportCallbackEXT( VkAllocationCallbacks const * callbacks
			, VkInstance instance
			, VkDebugReportCallbackCreateInfoEXT createInfo );
		void report( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const noexcept;

		void report( GlDebugSource source
			, GlDebugType type
			, uint32_t id
			, GlDebugSeverity severity
			, int length
			, const char * const message )const noexcept;
		void report( uint32_t id
			, GlDebugCategory category
			, GlDebugSeverity severity
			, int length
			, const char * const message )const noexcept;

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

	private:
		VkInstance m_instance;
		VkDebugReportCallbackCreateInfoEXT m_createInfo;
	};

#endif
}
