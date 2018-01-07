/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Texture_HPP___
#define ___VkRenderer_Texture_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Renderer/Texture.hpp>

#include <Utils/Vec2.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Texture
		: public renderer::Texture
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, utils::PixelFormat format
			, utils::IVec2 const & dimensions
			, VkImage image );
		/**
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*/
		void setImage( utils::PixelFormat format
			, utils::IVec2 const & size
			, renderer::ImageUsageFlags usageFlags = renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eSampled
			, renderer::ImageTiling tiling = renderer::ImageTiling::eOptimal )override;
		/**
		*\brief
		*	Génère les mipmaps de la texture.
		*/
		void generateMipmaps()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout général.
		*\param[in] accessFlags
		*	Les accès voulus, une fois que la transition est effectuée.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeGeneralLayout( renderer::AccessFlags accessFlags )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeTransferDestination()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeTransferSource()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeShaderInputResource()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\remarks
		*	Spécifique aux images prondeur et/ou stencil.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeDepthStencilReadOnly()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeColourAttachment()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur/stencil.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeDepthStencilAttachment()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de presentation.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makePresentSource()const override;
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
		/**
		*\~french
		*\brief
		*	Opérateur de conversion implicite vers VkImage.
		*\~english
		*\brief
		*	VkImage implicit cast operator.
		*/
		inline operator VkImage const &( )const
		{
			return m_image;
		}

	private:
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition de l'image vers un autre layout.
		*\param[in] layout
		*	Le layout vers lequel on fait la transition.
		*\param[in] queueFamily
		*	La file référençant l'image après la transition.
		*\param[in] dstAccessMask
		*	Les accès voulus, une fois que la transition est effectuée.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a layout transition memory barrier.
		*\param[in] layout
		*	The destination layout.
		*\param[in] queueFamily
		*	The queue referencing the image after the transition.
		*\param[in] dstAccessMask
		*	The wanted access after the transition is done.
		*\return
		*	The memory barrier.
		*/
		renderer::ImageMemoryBarrier doMakeLayoutTransition( renderer::ImageLayout layout
			, uint32_t queueFamily
			, renderer::AccessFlags dstAccessMask )const;

	private:
		Device const & m_device;
		VkImage m_image{};
		TextureViewPtr m_view;
		ImageStoragePtr m_storage;
		bool m_owner{};
		mutable renderer::AccessFlags m_currentAccessMask{};
		mutable renderer::ImageLayout m_currentLayout{};
		mutable uint32_t m_currentQueueFamily{ 0 };
	};
}

#endif
