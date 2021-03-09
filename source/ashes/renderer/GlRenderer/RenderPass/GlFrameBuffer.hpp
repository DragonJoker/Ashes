/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"
#include "renderer/GlRenderer/Enum/GlAttachmentPoint.hpp"
#include "renderer/GlRenderer/Enum/GlAttachmentType.hpp"

namespace ashes::gl
{
	bool isSRGBFormat( VkFormat format );
	GlAttachmentPoint getAttachmentPoint( VkImageAspectFlags aspectMask );
	GlAttachmentPoint getAttachmentPoint( VkImageView texture );
	GlAttachmentType getAttachmentType( VkImageAspectFlags aspectMask );
	GlAttachmentType getAttachmentType( VkImageView texture );
	void checkCompleteness( VkSwapchainKHR swapchain
		, GLenum status );
	void checkCompleteness( VkFramebuffer framebuffer
		, GLenum status );
	FboAttachment initialiseAttachment( VkDevice device
		, VkImageSubresourceLayers & subresource
		, VkImage image
		, uint32_t layer
		, VkImageView & view );

	class Framebuffer
		: public IcdObject
	{
	public:
		Framebuffer( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkFramebufferCreateInfo createInfo );
		Framebuffer( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, GLuint name );
		~Framebuffer();

		std::vector< GlAttachmentPoint > getDrawBuffers( ArrayView < VkAttachmentReference const > const & attaches )const;
		std::vector< GlAttachmentPoint > getDrawBuffers( ArrayView < VkAttachmentReference > const & attaches )const;

		bool hasOnlySwapchainImage()const;
		bool hasSwapchainImage()const;

		using IcdObject::getInternal;
		
		bool isEmpty()const
		{
			return m_renderableAttaches.empty()
				|| m_attachments.empty();
		}
		
		GLuint & getInternal()
		{
			return m_internal;
		}

		auto const & getAttachments()const
		{
			return m_attachments;
		}

		auto const & getRenderableAttaches()const
		{
			return m_renderableAttaches;
		}

		auto const & getResolveAttaches()const
		{
			return m_resolveAttaches;
		}

		auto const & getMsColourAttaches()const
		{
			return m_colourMsAttaches;
		}

		auto const & getColourAttaches()const
		{
			return m_colourAttaches;
		}

		auto const & getAllColourAttaches()const
		{
			return m_allColourAttaches;
		}

		bool hasDepthStencilAttach()const
		{
			return bool( m_depthStencilAttach );
		}

		auto const & getDepthStencilAttach()const
		{
			assert( hasDepthStencilAttach() );
			return m_depthStencilAttach.value();
		}

		bool isSRGB()const
		{
			return m_srgb;
		}

		VkExtent2D getDimensions()const
		{
			return m_dimensions;
		}

		uint32_t getWidth()const
		{
			return m_dimensions.width;
		}

		uint32_t getHeight()const
		{
			return m_dimensions.height;
		}

		bool isMultisampled()const
		{
			return m_multisampled;
		}

		CmdList const & getBindAttaches()const
		{
			return m_bindAttaches;
		}

		VkRenderPass getRenderPass()const
		{
			return m_renderPass;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		void doInitialiseAttaches();
		void doBindAttaches();
		void doCreateFramebuffer();
		void doInitialiseAttach( FboAttachment attach
			, bool multisampled
			, bool isSRGB );

	private:
		VkDevice m_device;
		VkFramebufferCreateFlags m_flags;
		VkRenderPass m_renderPass;
		VkImageViewArray m_attachments;
		VkExtent2D m_dimensions;
		uint32_t m_layers;
		FboAttachmentArray m_renderableAttaches;
		FboAttachmentArray m_resolveAttaches;
		FboAttachmentArray m_allColourAttaches;
		FboAttachmentArray m_colourAttaches;
		FboAttachmentArray m_colourMsAttaches;
		Optional< FboAttachment > m_depthStencilAttach;
		Optional< FboAttachment > m_depthStencilMsAttach;
		mutable std::vector< GlAttachmentPoint > m_drawBuffers;
		CmdList m_bindAttaches;
		bool m_srgb{ false };
		bool m_multisampled{ false };
	};
}
