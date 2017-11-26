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
	*	Sens dans lequel les faces sont considérées comme faisant face à la caméra.
	*/
	enum class FrontFace
		: uint32_t
	{
		eCounterClockwise,
		eClockwise,
	};
	/**
	*\brief
	*	Convertit un renderer::FrontFace en VkFrontFace.
	*\param[in] value
	*	Le renderer::FrontFace.
	*\return
	*	Le VkFrontFace.
	*/
	VkFrontFace convert( FrontFace const & value );
}
