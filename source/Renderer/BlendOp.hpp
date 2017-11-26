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
	*	Opérateurs de mélange.
	*/
	enum class BlendOp
		: uint32_t
	{
		eAdd,
		eSubtract,
		eReverseSubtract,
		eMin,
		eMax,
	};
	/**
	*\brief
	*	Convertit un renderer::BlendOp en VkBlendOp.
	*\param[in] value
	*	Le renderer::BlendOp.
	*\return
	*	Le VkBlendOp.
	*/
	VkBlendOp convert( BlendOp const & value );
}
