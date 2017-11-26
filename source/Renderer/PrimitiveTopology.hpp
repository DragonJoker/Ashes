/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

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
		VkLib_EnumBounds( ePointList )
	};
	/**
	*\brief
	*	Convertit un renderer::PrimitiveTopology en VkPrimitiveTopology.
	*\param[in] format
	*	Le renderer::PrimitiveTopology.
	*\return
	*	Le VkPrimitiveTopology.
	*/
	VkPrimitiveTopology convert( PrimitiveTopology const & topology );
}
