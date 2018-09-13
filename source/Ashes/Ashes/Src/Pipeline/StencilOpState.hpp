/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_StencilOpState_HPP___
#define ___Ashes_StencilOpState_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Stencil operation state.
	*\~french
	*\brief
	*	Etat pour une opération stencil.
	*/
	struct StencilOpState
	{
		StencilOp failOp = StencilOp::eKeep;
		StencilOp passOp = StencilOp::eKeep;
		StencilOp depthFailOp = StencilOp::eKeep;
		CompareOp compareOp = CompareOp::eAlways;
		uint32_t compareMask = 0xFFFFFFFFu;
		uint32_t writeMask = 0xFFFFFFFFu;
		uint32_t reference = 0u;
	};

	inline bool operator==( StencilOpState const & lhs, StencilOpState const & rhs )
	{
		return lhs.failOp == rhs.failOp
			&& lhs.passOp == rhs.passOp
			&& lhs.depthFailOp == rhs.depthFailOp
			&& lhs.compareOp == rhs.compareOp
			&& lhs.compareMask == rhs.compareMask
			&& lhs.writeMask == rhs.writeMask
			&& lhs.reference == rhs.reference;
	}

	inline bool operator!=( StencilOpState const & lhs, StencilOpState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
