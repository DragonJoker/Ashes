/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_StencilOp_HPP___
#define ___Renderer_StencilOp_HPP___
#pragma once

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
}

#endif
