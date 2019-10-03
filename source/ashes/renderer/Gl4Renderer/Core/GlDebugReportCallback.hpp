/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
#if VK_EXT_debug_utils

	class DebugUtilsMessengerEXT
	{
	public:
		DebugUtilsMessengerEXT( VkInstance instance
			, VkDebugUtilsMessengerCreateInfoEXT createInfo );
		~DebugUtilsMessengerEXT();
		void submit( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData
			, void * userData );

		void report( GlDebugSource source
			, GlDebugType type
			, uint32_t id
			, GlDebugSeverity severity
			, int length
			, const char * const message );
		void report( uint32_t id
			, GlDebugCategory category
			, GlDebugSeverity severity
			, int length
			, const char * const message );

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerCreateInfoEXT m_createInfo;
	};

#endif
#if VK_EXT_debug_report

	class DebugReportCallbackEXT
	{
	public:
		DebugReportCallbackEXT( VkInstance instance
			, VkDebugReportCallbackCreateInfoEXT createInfo );
		~DebugReportCallbackEXT();
		void report( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );

		void report( GlDebugSource source
			, GlDebugType type
			, uint32_t id
			, GlDebugSeverity severity
			, int length
			, const char * const message );
		void report( uint32_t id
			, GlDebugCategory category
			, GlDebugSeverity severity
			, int length
			, const char * const message );

	private:
		VkInstance m_instance;
		VkDebugReportCallbackCreateInfoEXT m_createInfo;
	};

#endif
}
