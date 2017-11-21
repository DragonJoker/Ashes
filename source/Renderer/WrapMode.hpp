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
	*	Enumération des modes de wrap.
	*@~english
	*@brief
	*	Wrap modes enumeration.
	*/
	enum class WrapMode
	{
		eRepeat,
		eMirroredRepeat,
		eClampToEdge,
		eClampToBorder,
		eMirrorClampToEdge,
		VkLib_EnumBounds( eRepeat )
	};
}
