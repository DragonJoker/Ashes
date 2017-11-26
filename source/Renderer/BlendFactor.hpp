/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Les facteurs de mélange.
	*/
	enum class BlendFactor
		: uint32_t
	{
		eZero,
		eOne,
		eSrcColour,
		eInvSrcColour,
		eDstColour,
		eInvDstColour,
		eSrcAlpha,
		eInvSrcAlpha,
		eDstAlpha,
		eInvDstAlpha,
		eConstantColour,
		eInvConstantColour,
		eConstantAlpha,
		eInvConstantAlpha,
		eSrcAlphaSaturate,
		eSrc1Colour,
		eInvSrc1Colour,
		eSrc1Alpha,
		eInvSrc1Alpha,
	};
	/**
	*\brief
	*	Convertit un renderer::BlendFactor en VkBlendFactor.
	*\param[in] value
	*	Le renderer::BlendFactor.
	*\return
	*	Le VkBlendFactor.
	*/
	VkBlendFactor convert( BlendFactor const & value );
}
