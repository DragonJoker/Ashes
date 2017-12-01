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
	*	Décrit comment les commandes d'une sous-passe sont fournies.
	*/
	enum class SubpassContents
		: uint32_t
	{
		//! Aucun tampon de commandes secondaire.
		eInline,
		//! Uniquement des tampons de commandes secondaire.
		eSecondaryCommandBuffers,
	};
	/**
	*\brief
	*	Convertit un renderer::SubpassContents en VkSubpassContents.
	*\param[in] value
	*	Le renderer::SubpassContents.
	*\return
	*	Le VkSubpassContents.
	*/
	VkSubpassContents convert( SubpassContents const & value );
}
