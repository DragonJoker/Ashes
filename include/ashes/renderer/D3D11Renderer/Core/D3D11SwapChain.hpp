/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/SwapChain.hpp>

namespace ashes::d3d11
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
		*	Les informations de cr�ation.
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
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~SwapChain();
		/**
		*\copydoc	ashes::SwapChain::getImages
		*/
		ashes::ImagePtrArray getImages()const;
		/**
		*\copydoc	ashes::SwapChain::acquireNextImage
		*/
		ashes::Result acquireNextImage( uint64_t timeout
			, ashes::Semaphore const * semaphore
			, ashes::Fence const * fence
			, uint32_t & imageIndex )const;

		inline IDXGISwapChain * getSwapChain()const
		{
			return m_swapChain;
		}

	private:
		DXGI_SWAP_CHAIN_DESC doInitPresentParameters();

	protected:
		Device const & m_device;
		uint32_t m_currentBuffer{};
		IDXGISwapChain * m_swapChain;
		Surface const & m_surface;
	};
}
