/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Core/SwapChain.hpp>

namespace gl_renderer
{
	class SwapChain
		: public ashes::SwapChain
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		SwapChain( Device const & device
			, ashes::Extent2D const & size );
		/**
		*\copydoc	ashes::SwapChain::reset
		*/
		void reset( ashes::Extent2D const & size )override;
		/**
		*\copydoc	ashes::SwapChain::createFrameBuffers
		*/
		ashes::FrameBufferPtrArray createFrameBuffers( ashes::RenderPass const & renderPass )const override;
		/**
		*\copydoc	ashes::SwapChain::createCommandBuffers
		*/
		ashes::CommandBufferPtrArray createCommandBuffers()const override;
		/**
		*\copydoc	ashes::SwapChain::getResources
		*/
		ashes::RenderingResources * getResources()override;
		/**
		*\copydoc	ashes::SwapChain::present
		*/
		void present( ashes::RenderingResources & resources )override;
		/**
		*\copydoc	ashes::SwapChain::createDepthStencil
		*/
		void createDepthStencil( ashes::Format format )override;
		/**
		*\brief
		*	D�finit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void setClearColour( ashes::ClearColorValue const & value )override
		{
			m_clearColour = value;
		}
		/**
		*\brief
		*	D�finit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline ashes::ClearColorValue getClearColour()const override
		{
			return m_clearColour;
		}
		/**
		*\return
		*	Les dimensions de la swap chain.
		*/
		inline ashes::Extent2D getDimensions()const override
		{
			return m_dimensions;
		}
		/**
		*\return
		*	Les format des pixels de la swap chain.
		*/
		inline ashes::Format getFormat()const override
		{
			return m_format;
		}

	private:
		void doResetSwapChain();
		void doCreateBackBuffers();
		ashes::FrameBufferAttachmentArray doPrepareAttaches( ashes::AttachmentDescriptionArray const & attaches )const;

	private:
		Device const & m_device;
		ashes::ClearColorValue m_clearColour;
		ashes::Format m_format;
	};
}
