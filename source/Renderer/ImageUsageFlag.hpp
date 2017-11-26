/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des utilisation de tampon de commandes.
	*/
	enum class ImageUsageFlag
		: uint32_t
	{
		eTransferSrc = 0x00000001,
		eTransferDst = 0x00000002,
		eSampled = 0x00000004,
		eStorage = 0x00000008,
		eColourAttachment = 0x00000010,
		eDepthStencilAttachment = 0x00000020,
		eTransientAttachment = 0x00000040,
		eInputAttachment = 0x00000080,
	};
	VkLib_ImplementFlag( ImageUsageFlag )
	/**
	*\brief
	*	Convertit un renderer::ImageUsageFlags en VkImageUsageFlags.
	*\param[in] flags
	*	Le renderer::ImageUsageFlags.
	*\return
	*	Le VkImageUsageFlags.
	*/
	VkImageUsageFlags convert( ImageUsageFlags const & flags );
}
