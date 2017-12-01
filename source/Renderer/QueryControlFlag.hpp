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
	*	Enumération des contrôles de requête d'occlusion.
	*/
	enum class QueryControlFlag
		: uint32_t
	{
		ePrecise = 0x00000001,
	};
	VkLib_ImplementFlag( QueryControlFlag )
	/**
	*\brief
	*	Convertit un renderer::QueryControlFlags en VkQueryControlFlags.
	*\param[in] flags
	*	Le renderer::QueryControlFlags.
	*\return
	*	Le VkQueryControlFlags.
	*/
	VkQueryControlFlags convert( QueryControlFlags const & flags );
}
