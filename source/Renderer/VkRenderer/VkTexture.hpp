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

#include <Renderer/Image/Texture.hpp>

#include <Utils/Vec2.hpp>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Classe encapsulant le concept d'image Vulkan.
	*\remarks
	*	Gère la transition de layouts.
	*	Dépendant du fait que l'image provienne de la swap chain
	*	ou d'une ressource, la VkImage sera détruite par le parent
	*	correspondant.
	*\~english
	*\brief
	*	Class wrapping the Vulkan image concept.
	*\remarks
	*	Handles the layouts transition.
	*	Depending on wheter the image comes from a resource or a swap chain,
	*	The VkImage will be destroyed or not.
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
			, renderer::PixelFormat format
			, renderer::IVec2 const & dimensions
			, VkImage image );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, renderer::PixelFormat format
			, renderer::IVec2 const & dimensions
			, renderer::ImageUsageFlags usageFlags
			, renderer::ImageTiling tiling
			, renderer::MemoryPropertyFlags memoryFlags );
		/**
		*\~french
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire du tampon est mappée.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*\~english
		*\brief
		*	Maps the buffer's memory in RAM.
		*\param[in] offset
		*	The memory mapping starting offset.
		*\param[in] size
		*	The memory mappping size.
		*\param[in] flags
		*	The memory mapping flags.
		*\return
		*	\p nullptr if the mapping failed.
		*/
		renderer::Texture::Mapped lock( uint32_t offset
			, uint32_t size
			, VkMemoryMapFlags flags )const;
		/**
		*\~french
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*\param[in] modified
		*	Dit si le tampon a été modifié, et donc si la VRAM doit être mise à jour.
		*\~english
		*\brief
		*	Unmaps the buffer's memory from the RAM.
		*\param[in] size
		*	The mapped memory size.
		*\param[in] modified
		*	Tells it the buffer has been modified, and whether the VRAM must be updated or not.
		*/
		void unlock( uint32_t size
			, bool modified )const;
		/**
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*/
		void setImage( renderer::PixelFormat format
			, renderer::IVec2 const & size
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
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*/
		void doSetImage( renderer::PixelFormat format
			, renderer::IVec2 const & size
			, renderer::ImageUsageFlags usageFlags
			, renderer::ImageTiling tiling
			, renderer::MemoryPropertyFlags memoryFlags );
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
		ImageStoragePtr m_storage;
		bool m_owner{};
		mutable renderer::AccessFlags m_currentAccessMask{};
		mutable renderer::ImageLayout m_currentLayout{};
		mutable uint32_t m_currentQueueFamily{ 0 };
	};
}

#endif
