/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <RenderPass/ClearValue.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ClearValue en VkClearValue.
	*\param[in] value
	*	Le ashes::ClearValue.
	*\return
	*	Le VkClearValue.
	*/
	VkClearValue convert( ashes::ClearValue const & value );
	/**
	*\brief
	*	Convertit une ashes::ClearColorValue en VkClearColorValue.
	*\param[in] value
	*	La ashes::ClearColorValue.
	*\return
	*	La VkClearColorValue.
	*/
	VkClearColorValue convert( ashes::ClearColorValue const & colour );
	/**
	*\brief
	*	Convertit une VkClearColorValue en ashes::ClearColorValue.
	*\param[in] value
	*	La VkClearColorValue.
	*\return
	*	La ashes::ClearColorValue.
	*/
	ashes::ClearColorValue convert( VkClearColorValue const & colour );
	/**
	*\brief
	*	Convertit une ashes::DepthStencilClearValue en VkClearDepthStencilValue.
	*\param[in] value
	*	La ashes::DepthStencilClearValue.
	*\return
	*	La VkClearDepthStencilValue.
	*/
	VkClearDepthStencilValue convert( ashes::DepthStencilClearValue const & value );
	/**
	*\brief
	*	Convertit une VkClearDepthStencilValue en ashes::DepthStencilClearValue.
	*\param[in] value
	*	La VkClearDepthStencilValue.
	*\return
	*	La ashes::DepthStencilClearValue.
	*/
	ashes::ClearColorValue convert( VkClearColorValue const & colour );
}
