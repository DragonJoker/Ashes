/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des indicateurs de réinitalisation de tampon de commandes.
	*/
	enum class CommandBufferResetFlag
		: uint32_t
	{
		eReleaseResources = 0x00000001,
	};
	VkLib_ImplementFlag( CommandBufferResetFlag )
	/**
	*\brief
	*	Convertit un renderer::CommandBufferResetFlags en VkCommandBufferResetFlags.
	*\param[in] flags
	*	Le renderer::CommandBufferResetFlags.
	*\return
	*	Le VkCommandBufferResetFlags.
	*/
	VkCommandBufferResetFlags convert( CommandBufferResetFlags const & flags );
}
