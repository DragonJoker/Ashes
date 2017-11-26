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
	*	Opérateurs logiques.
	*/
	enum class StencilOp
		: uint32_t
	{
		eKeep,
		eZero,
		eReplace,
		eIncrementAndClamp,
		eDecrementAndClamp,
		eInvert,
		eIncrementAndWrap,
		eDecrementAndWrap,
	};
	/**
	*\brief
	*	Convertit un renderer::StencilOp en VkStencilOp.
	*\param[in] value
	*	Le renderer::StencilOp.
	*\return
	*	Le VkStencilOp.
	*/
	VkStencilOp convert( StencilOp const & value );
}
