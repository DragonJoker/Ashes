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
	enum class ColourComponentFlag
		: uint32_t
	{
		eR = 0x00000001,
		eG = 0x00000002,
		eB = 0x00000004,
		eA = 0x00000008,
	};
	VkLib_ImplementFlag( ColourComponentFlag )
	/**
	*\brief
	*	Convertit un renderer::ColourComponentFlag en VkColorComponentFlags.
	*\param[in] flags
	*	Le renderer::ColourComponentFlag.
	*\return
	*	Le VkColorComponentFlags.
	*/
	VkColorComponentFlags convert( ColourComponentFlags const & flags );
}
