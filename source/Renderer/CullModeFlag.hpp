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
	enum class CullModeFlag
		: uint32_t
	{
		eNone = 0x00000000,
		eFront = 0x00000001,
		eBack = 0x00000002,
		eFrontAndBack = 0x00000003,
	};
	VkLib_ImplementFlag( CullModeFlag )
	/**
	*\brief
	*	Convertit un renderer::CullModeFlags en VkCullModeFlags.
	*\param[in] flags
	*	Le renderer::CullModeFlags.
	*\return
	*	Le VkCullModeFlags.
	*/
	VkCullModeFlags convert( CullModeFlags const & flags );
}
