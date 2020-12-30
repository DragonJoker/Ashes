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
	void checkCompleteness( GLenum status );

	class Framebuffer
		: public IcdObject
	{
	public:
		Framebuffer( VkDevice device
			, VkFramebufferCreateInfo createInfo );
		Framebuffer( VkDevice device
			, GLuint name );
		~Framebuffer();

		UInt32Array getDrawBuffers( ArrayView < VkAttachmentReference const > const & attaches )const;
		UInt32Array getDrawBuffers( ArrayView < VkAttachmentReference > const & attaches )const;

		bool hasOnlySwapchainImage()const;
		bool hasSwapchainImage()const;

		using IcdObject::getInternal;
		
		inline GLuint & getInternal()
		{
			return m_internal;
		}

		inline auto const & getAttachments()const
		{
			return m_attachments;
		}

		inline auto const & getAllAttaches()const
		{
			return m_allAttaches;
		}

		inline auto const & getMsColourAttaches()const
		{
			return m_colourMsAttaches;
		}

		inline auto const & getColourAttaches()const
		{
			return m_colourAttaches;
		}

		inline auto const & getAllColourAttaches()const
		{
			return m_allColourAttaches;
		}

		inline bool hasDepthStencilAttach()const
		{
			return bool( m_depthStencilAttach );
		}

		inline auto const & getDepthStencilAttach()const
		{
			assert( hasDepthStencilAttach() );
			return m_depthStencilAttach.value();
		}

		inline bool isSRGB()const
		{
			return m_srgb;
		}

		inline VkExtent2D getDimensions()const
		{
			return m_dimensions;
		}

		inline uint32_t getWidth()const
		{
			return m_dimensions.width;
		}

		inline uint32_t getHeight()const
		{
			return m_dimensions.height;
		}

		inline bool isMultisampled()const
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
		void doInitialiseAttach( VkImageView view
			, uint32_t index );

	private:
		VkDevice m_device;
		VkFramebufferCreateFlags m_flags;
		VkRenderPass m_renderPass;
		VkImageViewArray m_attachments;
		VkExtent2D m_dimensions;
		uint32_t m_layers;
		FboAttachmentArray m_allAttaches;
		FboAttachmentArray m_allColourAttaches;
		FboAttachmentArray m_colourAttaches;
		FboAttachmentArray m_colourMsAttaches;
		Optional< FboAttachment > m_depthStencilAttach;
		Optional< FboAttachment > m_depthStencilMsAttach;
		mutable UInt32Array m_drawBuffers;
		CmdList m_bindAttaches;
		bool m_srgb{ false };
		bool m_multisampled{ false };
	};
}
