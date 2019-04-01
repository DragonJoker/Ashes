/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DebugMarkerObjectNameInfo_HPP___
#define ___Ashes_DebugMarkerObjectNameInfo_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies the parameters of a name to set on an object.
	*\~french
	*\brief
	*	Définit les paramètres de nom à définir pour un objet.
	*/
	struct DebugMarkerObjectNameInfo
	{
		/**
		*\~english
		*\brief
		*	Specifies the type of the object to be named.
		*\~french
		*\brief
		*	Définit le type de l'objet à nommer.
		*/
		DebugReportObjectType objectType;
		/**
		*\~english
		*\brief
		*	The object to be named.
		*\~french
		*\brief
		*	L'objet à nommer.
		*/
		void const * object;
		/**
		*\~english
		*\brief
		*	UTF-8 string specifying the name to apply to object.
		*\~french
		*\brief
		*	Une chaîne UTF-8 définissant le nom à donner à l'objet.
		*/
		std::string objectName;
	};
}

#endif
