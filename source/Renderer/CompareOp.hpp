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
	*	Opérateurs de comparaison.
	*/
	enum class CompareOp
		: uint32_t
	{
		eNever,
		eLess,
		eEqual,
		eLessEqual,
		eGreater,
		eNotEqual,
		eGreaterEqual,
		eAlways,
	};
	/**
	*\brief
	*	Convertit un renderer::CompareOp en VkCompareOp.
	*\param[in] value
	*	Le renderer::CompareOp.
	*\return
	*	Le VkCompareOp.
	*/
	VkCompareOp convert( CompareOp const & value );
}
