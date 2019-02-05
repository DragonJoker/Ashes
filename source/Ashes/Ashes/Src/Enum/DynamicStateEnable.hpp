/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DynamicStateEnable_HPP___
#define ___Ashes_DynamicStateEnable_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Indicates which dynamic state is taken from dynamic state command.
	*\~french
	*\brief
	*	Indique quelque état dynamique est pris depuis une commande d'état dynamique.
	*/
	enum class DynamicStateEnable
	{
		eViewport = 0,
		eScissor = 1,
		eLineWidth = 2,
		eDepthBias = 3,
		eBlendConstants = 4,
		eDepthBounds = 5,
		eStencilCompareMask = 6,
		eStencilWriteMask = 7,
		eStencilReference = 8,
	};
}

#endif
