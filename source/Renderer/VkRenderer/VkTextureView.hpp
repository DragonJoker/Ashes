/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_TextureView_HPP___
#define ___VkRenderer_TextureView_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Renderer/ImageSubresourceRange.hpp>

#include <Utils/Vec2.hpp>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image Vulkan.
	*\~english
	*\brief
	*	Vulkan image view wrapper.
	*/
	class TextureView
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] image
		*	L'image sur laquelle la vue est créée.
		*\param[in] format
		*	Le format des pixels de l'image.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] image
		*	The image from which the view is created.
		*\param[in] format
		*	The image's pixels format.
		*/
		TextureView( Device const & device
			, Texture const & image
			, utils::PixelFormat format );
		/**
		*\~french
		*\brief
		*	Constructeur par copie.
		*\~english
		*\brief
		*	Copy constructor.
		*/
		TextureView( TextureView const & ) = delete;
		/**
		*\~french
		*\brief
		*	Opérateur d'affectation par copie.
		*\~english
		*\brief
		*	Copy assignment operator.
		*/
		TextureView & operator=( TextureView const & ) = delete;
		/**
		*\~french
		*\brief
		*	Constructeur par déplacement.
		*\~english
		*\brief
		*	Move constructor.
		*/
		TextureView( TextureView && rhs )noexcept;
		/**
		*\~french
		*\brief
		*	Opérateur d'affectation par déplacement.
		*\~english
		*\brief
		*	Move assignment operator.
		*/
		TextureView & operator=( TextureView && rhs )noexcept;
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~TextureView();
		/**
		*\~french
		*\return
		*	Le format de l'image de la vue.
		*\~english
		*\return
		*	The view's image's pixels format.
		*/
		inline VkFormat getFormat()const
		{
			return m_format;
		}
		/**
		*\~french
		*\return
		*	L'image de la vue.
		*\~english
		*\return
		*	The view's image.
		*/
		inline Texture const & getImage()const
		{
			return m_image;
		}
		/**
		*\~french
		*\return
		*	Les informations de la ressource liée à l'image.
		*\~english
		*\return
		*	The image's resource information.
		*/
		inline renderer::ImageSubresourceRange const & getSubResourceRange()const
		{
			return m_subResourceRange;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkImageView.
		*\~english
		*\brief
		*	VkImageView implicit cast operator.
		*/
		inline operator VkImageView const &( )const
		{
			return m_view;
		}
		/**
		*\~french
		*\brief
		*	Echange les valeurs de deux ImageView.
		*\param[in] lhs, rhs
		*	Les valeurs à échanger.
		*\~english
		*\brief
		*	Swaps two ImageView.
		*\param[in,out] lhs, rhs
		*	The values to swap.
		*/
		void swap( TextureView & lhs, TextureView & rhs )noexcept
		{
			std::swap( lhs.m_format, rhs.m_format );
			std::swap( lhs.m_view, rhs.m_view );
		}

	private:
		Device const & m_device;
		Texture const & m_image;
		VkFormat m_format{};
		VkImageView m_view{};
		renderer::ImageSubresourceRange m_subResourceRange{};
	};
}

#endif
