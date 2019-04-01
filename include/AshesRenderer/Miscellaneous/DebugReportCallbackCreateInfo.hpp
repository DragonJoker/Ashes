/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DebugReportCallbackCreateInfo_HPP___
#define ___Ashes_DebugReportCallbackCreateInfo_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	using PFN_debugReportCallback = bool ( ASHES_API * )(
		DebugReportFlags flags,
		DebugReportObjectType objectType,
		uint64_t object,
		size_t location,
		int32_t messageCode,
		const char * pLayerPrefix,
		const char * pMessage,
		void * pUserData );
	/**
	*\~english
	*\brief
	*	Holds the data used to create a debug report callback.
	*\~french
	*\brief
	*	Contient les données utilisées pour créer un callback de rapport de debug.
	*/
	struct DebugReportCallbackCreateInfo
	{
		DebugReportFlags flags;
		PFN_debugReportCallback pfnCallback;
		void * pUserData;
	};
}

#endif
