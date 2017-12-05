/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkTexture.hpp"

#include <VkLib/BackBuffer.hpp>
#include <Renderer/BackBuffer.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe encapsulant une image venant d'une swap chain.
	*/
	class BackBuffer
		: public renderer::BackBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] swapchain
		*	La swapchain.
		*\param[in] imageIndex
		*	L'index de l'image, parmi les images de la swap chain.
		*/
		BackBuffer( renderer::Device const & device
			, renderer::SwapChain const & swapchain
			, uint32_t imageIndex );
		/**
		*\return
		*	La texture du back buffer.
		*/
		inline Texture const & getTexture()const override
		{
			return m_texture;
		}
		/**
		*\return
		*	Le vk::BackBuffer.
		*/
		inline vk::BackBuffer const & getBackBuffer()const
		{
			return m_backBuffer;
		}

	private:
		vk::BackBuffer & m_backBuffer;
		Texture const & m_texture;
	};
}
