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
	*	Enumération des tilings d'image.
	*/
	enum class ImageTiling
	{
		eOptimal,
		eLinear,
	};
	/**
	*\brief
	*	Convertit un renderer::ImageTiling en VkImageTiling.
	*\param[in] value
	*	Le renderer::ImageTiling.
	*\return
	*	Le VkImageTiling.
	*/
	VkImageTiling convert( ImageTiling const & value );
}
