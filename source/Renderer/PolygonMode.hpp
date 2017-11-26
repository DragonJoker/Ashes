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
	*	Modes d'affichage des polygones.
	*/
	enum class PolygonMode
		: uint32_t
	{
		eFill,
		eLine,
		ePoint,
	};
	/**
	*\brief
	*	Convertit un renderer::PolygonMode en VkPolygonMode.
	*\param[in] value
	*	Le renderer::PolygonMode.
	*\return
	*	Le VkPolygonMode.
	*/
	VkPolygonMode convert( PolygonMode const & value );
}
