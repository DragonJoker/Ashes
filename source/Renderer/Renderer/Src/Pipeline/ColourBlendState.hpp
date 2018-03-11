/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_ColourBlendState_HPP___
#define ___Renderer_ColourBlendState_HPP___
#pragma once

#include "Pipeline/ColourBlendStateAttachment.hpp"

namespace renderer
{
	/**
	*\brief
	*	Etat de mélange.
	*/
	struct ColourBlendState
	{
		bool logicOpEnable = false;
		LogicOp logicOp = LogicOp::eCopy;
		std::array< float, 4u > blendConstants{ 0.0f, 0.0f, 0.0f, 0.0f };
		ColourBlendStateAttachmentArray attachs;
		/**
		*\return
		*	Un ColourBlendState avec une attache couleur par défaut.
		*/
		static ColourBlendState const & createDefault();
	};

	inline bool operator==( ColourBlendState const & lhs, ColourBlendState const & rhs )
	{
		auto result = lhs.logicOpEnable == rhs.logicOpEnable
			&& lhs.logicOp == rhs.logicOp
			&& lhs.blendConstants[0] == rhs.blendConstants[0]
			&& lhs.blendConstants[1] == rhs.blendConstants[1]
			&& lhs.blendConstants[2] == rhs.blendConstants[2]
			&& lhs.blendConstants[3] == rhs.blendConstants[3]
			&& lhs.attachs.size() == rhs.attachs.size();

		for ( size_t i = 0; i < lhs.attachs.size() && result; ++i )
		{
			result = lhs.attachs[i] == rhs.attachs[i];
		}

		return result;
	}

	inline bool operator!=( ColourBlendState const & lhs, ColourBlendState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
