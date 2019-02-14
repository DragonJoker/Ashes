/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ColourBlendStateAttachment_HPP___
#define ___Ashes_ColourBlendStateAttachment_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a pipeline colour blend attachment state.
	*\~french
	*\brief
	*	Définit une attache de mélange couleur dans le pipeline.
	*/
	struct ColourBlendStateAttachment
	{
		bool blendEnable = false;
		BlendFactor srcColorBlendFactor = BlendFactor::eOne;
		BlendFactor dstColorBlendFactor = BlendFactor::eZero;
		BlendOp colorBlendOp = BlendOp::eAdd;
		BlendFactor srcAlphaBlendFactor = BlendFactor::eOne;
		BlendFactor dstAlphaBlendFactor = BlendFactor::eZero;
		BlendOp alphaBlendOp = BlendOp::eAdd;
		ColourComponentFlags colorWriteMask = ColourComponentFlag::eR | ColourComponentFlag::eG | ColourComponentFlag::eB | ColourComponentFlag::eA;
	};

	inline bool operator==( ColourBlendStateAttachment const & lhs, ColourBlendStateAttachment const & rhs )
	{
		return lhs.blendEnable == rhs.blendEnable
			&& lhs.srcColorBlendFactor == rhs.srcColorBlendFactor
			&& lhs.dstColorBlendFactor == rhs.dstColorBlendFactor
			&& lhs.colorBlendOp == rhs.colorBlendOp
			&& lhs.srcAlphaBlendFactor == rhs.srcAlphaBlendFactor
			&& lhs.dstAlphaBlendFactor == rhs.dstAlphaBlendFactor
			&& lhs.alphaBlendOp == rhs.alphaBlendOp
			&& lhs.colorWriteMask == rhs.colorWriteMask
			;
	}

	inline bool operator!=( ColourBlendStateAttachment const & lhs, ColourBlendStateAttachment const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
