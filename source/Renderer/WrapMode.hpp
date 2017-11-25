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
	*	Enumération des modes de wrap.
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
	/**
	*\brief
	*	Convertit un renderer::WrapMode en VkSamplerAddressMode.
	*\param[in] mode
	*	Le renderer::WrapMode.
	*\return
	*	Le VkSamplerAddressMode.
	*/
	VkSamplerAddressMode convert( WrapMode const & mode );
}
