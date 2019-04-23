/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	/**
	*\~english
	*\brief
	*	The debug report callback class.
	*\~french
	*\brief
	*	Classe de callback de rapport de debug.
	*/
	class DebugReportCallbackEXT
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructor.
		*\~french
		*\brief
		*	Constructeur.
		*/
		DebugReportCallbackEXT( VkInstance instance
			, VkDebugReportCallbackCreateInfoEXT createInfo );
		/**
		*\~french
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
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
}
