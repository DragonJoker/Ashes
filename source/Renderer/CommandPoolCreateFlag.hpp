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
	enum class CommandPoolCreateFlag
		: uint32_t
	{
		eTransient = 0x00000001,
		eResetCommandBuffer = 0x00000002,
	};
	VkLib_ImplementFlag( CommandPoolCreateFlag )
	/**
	*\brief
	*	Convertit un renderer::CommandPoolCreateFlags en VkCommandPoolCreateFlags.
	*\param[in] flags
	*	Le renderer::CommandPoolCreateFlags.
	*\return
	*	Le VkCommandPoolCreateFlags.
	*/
	VkCommandPoolCreateFlags convert( CommandPoolCreateFlags const & flags );
}
