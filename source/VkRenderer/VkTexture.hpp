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

#include <VkLib/Image.hpp>
#include <VkLib/Sampler.hpp>

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
		Texture( renderer::Device const & device );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( renderer::Device const & device
			, vk::Image const & image );
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
		*\return
		*	L'image vulkan.
		*/
		inline vk::Image const & getImage()const noexcept
		{
			assert( m_nonOwnedTexture );
			return *m_nonOwnedTexture;
		}

	private:
		vk::ImagePtr m_ownedTexture;
		vk::Image const * m_nonOwnedTexture{ nullptr };
	};
}

#endif
