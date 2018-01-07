/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/ClearValue.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ClearValue en VkClearValue.
	*\param[in] value
	*	Le renderer::ClearValue.
	*\return
	*	Le VkClearValue.
	*/
	VkClearValue convert( renderer::ClearValue const & value );
	/**
	*\brief
	*	Convertit une utils::RgbaColour en VkClearColorValue.
	*\param[in] value
	*	La utils::RgbaColour.
	*\return
	*	La VkClearColorValue.
	*/
	VkClearColorValue convert( utils::RgbaColour const & colour );
	/**
	*\brief
	*	Convertit une VkClearColorValue en utils::RgbaColour.
	*\param[in] value
	*	La VkClearColorValue.
	*\return
	*	La utils::RgbaColour.
	*/
	utils::RgbaColour convert( VkClearColorValue const & colour );
	/**
	*\brief
	*	Convertit une renderer::DepthStencilClearValue en VkClearDepthStencilValue.
	*\param[in] value
	*	La renderer::DepthStencilClearValue.
	*\return
	*	La VkClearDepthStencilValue.
	*/
	VkClearDepthStencilValue convert( renderer::DepthStencilClearValue const & value );
	/**
	*\brief
	*	Convertit une VkClearDepthStencilValue en renderer::DepthStencilClearValue.
	*\param[in] value
	*	La VkClearDepthStencilValue.
	*\return
	*	La renderer::DepthStencilClearValue.
	*/
	utils::RgbaColour convert( VkClearColorValue const & colour );
}
