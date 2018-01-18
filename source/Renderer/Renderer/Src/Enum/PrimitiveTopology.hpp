/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_PrimitiveTopology_HPP___
#define ___Renderer_PrimitiveTopology_HPP___
#pragma once

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Enumération des types de primitives.
	*\~english
	*\brief
	*	Primitive types enumeration.
	*/
	enum class PrimitiveTopology
		: uint32_t
	{
		ePointList,
		eLineList,
		eLineStrip,
		eTriangleList,
		eTriangleStrip,
		eTriangleFan,
		eLineListWithAdjacency,
		eLineStripWithAdjacency,
		eTriangleListWithAdjacency,
		eTriangleStripWithAdjacency,
		ePatchList,
		Utils_EnumBounds( ePointList )
	};
}

#endif
