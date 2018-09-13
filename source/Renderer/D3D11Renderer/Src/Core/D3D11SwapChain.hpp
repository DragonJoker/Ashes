/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Core/SwapChain.hpp>

namespace d3d11_renderer
{
	class SwapChain
		: public renderer::SwapChain
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] size
		*	Les dimensions de la surface de rendu.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] size
		*	The render surface dimensions.
		*/
		SwapChain( Device const & device
			, renderer::Extent2D const & size );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~SwapChain();
		/**
		*\copydoc	renderer::SwapChain::reset
		*/
		void reset( renderer::Extent2D const & size )override;
		/**
		*\copydoc	renderer::SwapChain::createFrameBuffers
		*/
		renderer::FrameBufferPtrArray createFrameBuffers( renderer::RenderPass const & renderPass )const override;
		/**
		*\copydoc	renderer::SwapChain::createCommandBuffers
		*/
		renderer::CommandBufferPtrArray createCommandBuffers()const override;
		/**
		*\copydoc	renderer::SwapChain::getResources
		*/
		renderer::RenderingResources * getResources()override;
		/**
		*\copydoc	renderer::SwapChain::present
		*/
		void present( renderer::RenderingResources & resources )override;
		/**
		*\copydoc	renderer::SwapChain::createDepthStencil
		*/
		void createDepthStencil( renderer::Format format )override;
		/**
		*\~french
		*\return
		*	Les tampons d'images.
		*\~english
		*\return
		*	The back buffers.
		*/
		inline auto const & getBackBuffers()const
		{
			return m_backBuffers;
		}
		/**
		*\~french
		*\brief
		*	D�finit couleur de vidage.
		*\param[in] colour
		*	La nouvelle valeur.
		*\~english
		*\brief
		*	Defines the clear colour.
		*\param[in] colour
		*	The new value.
		*/
		inline void setClearColour( renderer::ClearColorValue const & value )override
		{
			m_clearColour = value;
		}
		/**
		*\~french
		*\return
		*	La couleur de vidage.
		*\~english
		*\return
		*	The clear colour.
		*/
		inline renderer::ClearColorValue getClearColour()const override
		{
			return m_clearColour;
		}
		/**
		*\~french
		*\return
		*	Les dimensions de la swap chain.
		*\~french
		*\return
		*	The swap chain dimensions.
		*/
		inline renderer::Extent2D getDimensions()const override
		{
			return m_dimensions;
		}
		/**
		*\~french
		*\return
		*	Le format des images de la swap chain.
		*\~english
		*\return
		*	The swap chain's images pixels format.
		*/
		inline renderer::Format getFormat()const override
		{
			return m_format;
		}

	private:
		void doInitPresentParameters();
		void doCreateSwapChain();
		void doCreateBackBuffers();
		renderer::FrameBufferAttachmentArray doPrepareAttaches( uint32_t backBuffer
			, renderer::AttachmentDescriptionArray const & attaches )const;
		void doResetSwapChain();

	protected:
		Device const & m_device;
		renderer::Format m_format{};
		uint32_t m_currentBuffer{};
		renderer::ClearColorValue m_clearColour{};
		IDXGISwapChain * m_swapChain;
		DXGI_SWAP_CHAIN_DESC m_desc;
		DXGI_SWAP_CHAIN_DESC1 m_desc1;
	};
}
