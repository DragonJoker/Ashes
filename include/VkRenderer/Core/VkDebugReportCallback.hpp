/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Ashes/Core/DebugReportCallback.hpp>

namespace vk_renderer
{
	/**
	*\~english
	*\brief
	*	The debug report callback class.
	*\~french
	*\brief
	*	Classe de callback de rapport de debug.
	*/
	class DebugReportCallback
		: public ashes::DebugReportCallback
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
		DebugReportCallback( Instance const & instance
			, ashes::DebugReportCallbackCreateInfo createInfo );
		/**
		*\~french
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~DebugReportCallback();

	protected:
		Instance const & m_vkInstance;
		VkDebugReportCallbackCreateInfoEXT m_vkCreateInfo;
		VkDebugReportCallbackEXT m_callback;
	};
}
