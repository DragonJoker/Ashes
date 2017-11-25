/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des layouts d'image.
	*/
	enum class ImageLayout
	{
		eUndefined = 0,
		eGeneral = 1,
		eColourAttachmentOptimal = 2,
		eDepthStencilAttachmentOptimal = 3,
		eDepthStencilReadOnlyOptimal = 4,
		eShaderReadOnlyOptimal = 5,
		eTransferSrcOptimal = 6,
		eTransferDstOptimal = 7,
		ePreinitialised = 8,
		VkLib_EnumBounds( eUndefined ),
		ePresentSrc = 1000001002,
	};
	/**
	*\brief
	*	Convertit un renderer::ImageLayout en VkImageLayout.
	*\param[in] layout
	*	Le renderer::ImageLayout.
	*\return
	*	Le VkImageLayout.
	*/
	VkImageLayout convert( ImageLayout const & layout );
}
