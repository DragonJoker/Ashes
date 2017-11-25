/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "Texture.hpp"

#include <VkLib/BackBuffer.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe encapsulant une image venant d'une swap chain.
	*/
	class BackBuffer
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
		BackBuffer( Device const & device
			, SwapChain const & swapchain
			, uint32_t imageIndex );
		/**
		*\return
		*	La texture du back buffer.
		*/
		inline Texture const & getTexture()const
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
