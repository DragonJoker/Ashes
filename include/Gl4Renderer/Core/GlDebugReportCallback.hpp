/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Ashes/Core/DebugReportCallback.hpp>

namespace gl_renderer
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
		Instance const & m_instance;
	};
}
