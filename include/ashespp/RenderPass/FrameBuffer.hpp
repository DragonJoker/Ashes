/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_FrameBuffer_HPP___
#define ___AshesPP_FrameBuffer_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Class wrapping the concept of a Framebuffer.
	*/
	class FrameBuffer
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Creates a framebuffer compatible with the given render pass.
		*\param[in] renderPass
		*	The render pass, containing the attachments.
		*\param[in] dimensions
		*	The frame buffer dimensions.
		*\param[in] views
		*	The views wanted for the framebuffer.
		*/
		FrameBuffer( Device const & device
			, std::string const & debugName
			, VkRenderPass renderPass
			, VkExtent2D const & dimensions
			, ImageViewCRefArray views
			, uint32_t layers = 1u );
		/**
		*\brief
		*	Creates a framebuffer compatible with the given render pass.
		*\param[in] renderPass
		*	The render pass, containing the attachments.
		*\param[in] dimensions
		*	The frame buffer dimensions.
		*\param[in] views
		*	The views wanted for the framebuffer.
		*/
		FrameBuffer( Device const & device
			, VkRenderPass renderPass
			, VkExtent2D const & dimensions
			, ImageViewCRefArray views
			, uint32_t layers = 1u );
		/**
		*\brief
		*	Creates a framebuffer compatible with the given render pass.
		*\param[in] renderPass
		*	The render pass, containing the attachments.
		*\param[in] dimensions
		*	The frame buffer dimensions.
		*\param[in] views
		*	The views wanted for the framebuffer.
		*/
		FrameBuffer( RenderPass const & renderPass
			, VkExtent2D const & dimensions
			, ImageViewCRefArray views
			, uint32_t layers = 1u );
		/**
		*\brief
		*	Creates a framebuffer compatible with the given render pass.
		*\param[in] renderPass
		*	The render pass, containing the attachments.
		*\param[in] dimensions
		*	The frame buffer dimensions.
		*\param[in] views
		*	The views wanted for the framebuffer.
		*/
		FrameBuffer( std::string const & debugName
			, RenderPass const & renderPass
			, VkExtent2D const & dimensions
			, ImageViewCRefArray views
			, uint32_t layers = 1u );
		/**
		*\brief
		*	Creates a framebuffer compatible with the given render pass.
		*\param[in] renderPass
		*	The render pass, containing the attachments.
		*\param[in] dimensions
		*	The frame buffer dimensions.
		*\param[in] views
		*	The views wanted for the framebuffer.
		*/
		FrameBuffer( Device const & device
			, std::string const & debugName
			, VkFramebufferCreateInfo const & createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~FrameBuffer();
		/**
		*\name
		*	Getters.
		**/
		/**@{*/
		inline VkExtent2D const & getDimensions()const
		{
			return m_dimensions;
		}

		inline auto begin()const
		{
			return m_views.begin();
		}

		inline auto end()const
		{
			return m_views.end();
		}
		/**@}*/
		/**
		*\brief
		*	VkFramebuffer implicit cast operator.
		*/
		inline operator VkFramebuffer const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkExtent2D m_dimensions;
		ImageViewCRefArray m_views;
		VkFramebuffer m_internal{};
	};
}

#endif
