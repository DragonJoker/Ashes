/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*@~french
	*@brief
	*	Enumération des filtres.
	*@~english
	*@brief
	*	Filters enumeration.
	*/
	enum class Filter
	{
		eNearest,
		eLinear,
		VkLib_EnumBounds( eNearest )
	};
	/**
	*\brief
	*	Convertit un renderer::Filter en VkFilter.
	*\param[in] filter
	*	Le renderer::Filter.
	*\return
	*	Le VkFilter.
	*/
	VkFilter convert( Filter filter );
}
