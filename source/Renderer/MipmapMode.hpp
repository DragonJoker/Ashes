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
	*	Enumération des modes de mipmap.
	*@~english
	*@brief
	*	Mipmap modes enumeration.
	*/
	enum class MipmapMode
	{
		eNearest,
		eLinear,
		VkLib_EnumBounds( eNearest )
	};
}
