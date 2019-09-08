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
	*\~english
	*\brief
	*	Class wrapping the concept of a Framebuffer.
	*\~french
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*/
	class FrameBuffer
	{
	public:
		/**
		*\~english
		*\brief
		*	Creates a framebuffer compatible with the given render pass.
		*\param[in] renderPass
		*	The render pass, containing the attachments.
		*\param[in] dimensions
		*	The frame buffer dimensions.
		*\param[in] views
		*	The views wanted for the framebuffer.
		*\~french
		*\brief
		*	Crée un FrameBuffer compatible avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu, contenant les attaches.
		*\param[in] dimensions
		*	Les dimensions du tampon d'images.
		*\param[in] views
		*	Les vues voulues pour le tampon d'images à créer.
		*/
		FrameBuffer( RenderPass const & renderPass
			, VkExtent2D const & dimensions
			, ImageViewCRefArray views );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
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
		*\~french
		*\brief
		*	Conversion implicite vers VkFramebuffer.
		*\~english
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
		VkFramebuffer m_internal{ VK_NULL_HANDLE };
	};
}

#endif
