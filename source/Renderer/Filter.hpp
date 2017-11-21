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
}
