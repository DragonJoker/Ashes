/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_Extent2D_HPP___
#define ___VkRenderer_Extent2D_HPP___
#pragma once

#include <Miscellaneous/Extent2D.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::Extent2D en VkExtent2D.
	*\param[in] mode
	*	Le renderer::Extent2D.
	*\return
	*	Le VkExtent2D.
	*/
	VkExtent2D convert( renderer::Extent2D const & value );
	/**
	*\brief
	*	Convertit un VkExtent2D en renderer::Extent2D.
	*\param[in] mode
	*	Le VkExtent2D.
	*\return
	*	Le renderer::Extent2D.
	*/
	renderer::Extent2D convert( VkExtent2D const & value );
}

#endif
