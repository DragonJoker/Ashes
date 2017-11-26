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
	enum class CommandBufferUsageFlag
		: uint32_t
	{
		eOneTimeSubmit = 0x00000001,
		eRenderPassContinue = 0x00000002,
		eSimultaneousUse = 0x00000004,
	};
	VkLib_ImplementFlag( CommandBufferUsageFlag )
	/**
	*\brief
	*	Convertit un renderer::CommandBufferUsageFlag en VkCommandBufferUsageFlags.
	*\param[in] flags
	*	Le renderer::CommandBufferUsageFlag.
	*\return
	*	Le VkCommandBufferUsageFlags.
	*/
	VkCommandBufferUsageFlags convert( CommandBufferUsageFlags const & flags );
}
