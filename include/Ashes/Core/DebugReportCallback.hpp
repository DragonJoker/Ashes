/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DebugReportCallback_HPP___
#define ___Ashes_DebugReportCallback_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"
#include "Ashes/Miscellaneous/DebugReportCallbackCreateInfo.hpp"

namespace ashes
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
	{
	protected:
		/**
		*\~french
		*\brief
		*	Constructor.
		*\~french
		*\brief
		*	Constructeur.
		*/
		DebugReportCallback( Instance const & instance
			, DebugReportCallbackCreateInfo createInfo );

	public:
		/**
		*\~french
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~DebugReportCallback();

	protected:
		Instance const & m_instance;
		DebugReportCallbackCreateInfo m_createInfo;

	};
}

#endif
