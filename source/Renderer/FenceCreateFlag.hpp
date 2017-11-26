/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class FenceCreateFlag
		: uint32_t
	{
		eSignaled = 0x00000001,
	};
	VkLib_ImplementFlag( FenceCreateFlag )
	/**
	*\brief
	*	Convertit un renderer::FenceCreateFlags en VkFenceCreateFlags.
	*\param[in] flags
	*	Le renderer::FenceCreateFlags.
	*\return
	*	Le VkFenceCreateFlags.
	*/
	VkFenceCreateFlags convert( FenceCreateFlags const & flags );
}
