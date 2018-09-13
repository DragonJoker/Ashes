/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SubpassContents_HPP___
#define ___Ashes_SubpassContents_HPP___
#pragma once

namespace ashes
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
}

#endif
