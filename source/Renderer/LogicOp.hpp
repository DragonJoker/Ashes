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
	enum class LogicOp
		: uint32_t
	{
		eClear,
		eAnd,
		eAndReverse,
		eCopy,
		eAndInverted,
		eNoOp,
		eXor,
		eOr,
		eNor,
		eEquivalent,
		eInvert,
		eOrReverse,
		eCopyInverted,
		eOrInverted,
		eNand,
		eSet,
	};
	/**
	*\brief
	*	Convertit un renderer::LogicOp en VkLogicOp.
	*\param[in] value
	*	Le renderer::LogicOp.
	*\return
	*	Le VkLogicOp.
	*/
	VkLogicOp convert( LogicOp const & value );
}
