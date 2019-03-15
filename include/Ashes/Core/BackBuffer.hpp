/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_BackBuffer_HPP___
#define ___Ashes_BackBuffer_HPP___
#pragma once

#include "Ashes/Image/Texture.hpp"
#include "Ashes/Image/TextureView.hpp"

namespace ashes
{
	/**
	*\brief
	*	Classe encapsulant une image venant d'une swap chain.
	*/
	class BackBuffer
	{
	protected:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device connection.
		*\param[in] image
		*	The swapchain image.
		*\param[in] view
		*	The swapchain image's view.
		*\param[in] imageIndex
		*	The swapchain image index, from its images.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] image
		*	L'image de la swapchain.
		*\param[in] view
		*	La vue sur l'image de la swapchain.
		*\param[in] imageIndex
		*	L'index de l'image, parmi les images de la swap chain.
		*/
		BackBuffer( Device const & device
			, TexturePtr image
			, TextureViewPtr view
			, uint32_t imageIndex );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~BackBuffer();
		/**
		*\~french
		*\return
		*	L'index de l'image dans la swap chain.
		*\~english
		*\return
		*	The image index, inside the swap chain.
		*/
		inline uint32_t getImageIndex()const
		{
			return m_imageIndex;
		}
		/**
		*\~french
		*\return
		*	L'image du back buffer.
		*\~english
		*\return
		*	The back buffer image.
		*/
		inline Texture const & getTexture()const
		{
			return *m_image;
		}
		/**
		*\~french
		*\return
		*	La vue sur l'image.
		*\~english
		*\return
		*	The image view.
		*/
		inline TextureView const & getView()const
		{
			return *m_view;
		}

	private:
		Device const & m_device;
		TexturePtr m_image;
		TextureViewPtr m_view;
		uint32_t m_imageIndex{ 0u };
	};
}

#endif
