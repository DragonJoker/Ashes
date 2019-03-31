/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Core/SwapChain.hpp>

namespace gl_renderer
{
	class SwapChain
		: public ashes::SwapChain
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] createInfo
		*	Les informations de création.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] createInfo
		*	The creation informations.
		*/
		SwapChain( Device const & device
			, ashes::SwapChainCreateInfo createInfo );
		/**
		*\copydoc	ashes::SwapChain::getImages
		*/
		ashes::ImagePtrArray getImages()const override;
		/**
		*\copydoc	ashes::SwapChain::acquireNextImage
		*/
		ashes::Result acquireNextImage( uint64_t timeout
			, ashes::Semaphore const * semaphore
			, ashes::Fence const * fence
			, uint32_t & imageIndex )const override;
		/**
		*\copydoc	ashes::SwapChain::createDepthStencil
		*/
		void createDepthStencil( ashes::Format format )override;

	private:
		Device const & m_device;
	};
}
