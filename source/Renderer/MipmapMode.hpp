/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des modes de mipmap.
	*/
	enum class MipmapMode
	{
		eNearest,
		eLinear,
		VkLib_EnumBounds( eNearest )
	};
	/**
	*\brief
	*	Convertit un renderer::MipmapMode en VkSamplerMipmapMode.
	*\param[in] mode
	*	Le renderer::MipmapMode.
	*\return
	*	Le VkSamplerMipmapMode.
	*/
	VkSamplerMipmapMode convert( MipmapMode mode );
}
