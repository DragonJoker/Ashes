/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Texture_HPP___
#define ___AshesPP_Texture_HPP___
#pragma once

#include "ashespp/Image/ImageCreateInfo.hpp"
#include "ashespp/Miscellaneous/DeviceMemory.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	A texture image.
	*\~french
	*\brief
	*	L'image d'une texture.
	*/
	class Image
	{
	public:
		/**
		*\~french
		*\brief
		*	Contient les informations d'une image mappée en RAM.
		*\~english
		*\brief
		*	Contains an image mapped in RAM informations.
		*/
		struct Mapped
		{
			uint8_t * data;
			uint64_t size;
			uint64_t rowPitch;
			uint64_t arrayPitch;
			uint64_t depthPitch;
		};

	private:
		Image( Image const & ) = delete;
		Image & operator=( Image const & ) = delete;

	public:
		Image( Image && rhs );
		Image & operator=( Image && rhs );
		Image();
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] createInfo
		*	Les informations de création de l'image.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The image creation informations.
		*/
		Image( Device const & device
			, ImageCreateInfo createInfo );
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] image
		*	L'image.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] image
		*	The image.
		*/
		Image( Device const & device
			, VkImage image );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Image();
		/**
		*\~english
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*\~french
		*\brief
		*	Lie ce tampon à l'objet mémoire donné.
		*\param[in] memory
		*	L'object mémoire de périphérique.
		*/
		void bindMemory( DeviceMemoryPtr memory );
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
		Mapped lock( uint32_t offset
			, uint32_t size
			, VkMemoryMapFlags flags )const;
		/**
		*\~english
		*\brief
		*	Invalidates the buffer content.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*\~french
		*\brief
		*	Invalide le contenu du tampon.
		*\param[in] offset
		*	L'offset de la mémoire mappée.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*/
		void invalidate( uint32_t offset
			, uint32_t size )const;
		/**
		*\~english
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*\~french
		*\brief
		*	Met à jour la VRAM.
		*\param[in] offset
		*	L'offset de la mémoire mappée.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*/
		void flush( uint32_t offset
			, uint32_t size )const;
		/**
		*\~english
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*\~french
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*/
		void unlock()const;
		/**
		*\~french
		*\brief
		*	Génère les mipmaps de la texture.
		*\param[in] commandBuffer
		*	Un tampon de commandes en cours d'enregistrement.
		*\~english
		*\brief
		*	Generates the texture mipmaps.
		*\param[in] commandBuffer
		*	A command buffer, in record state.
		*/
		void generateMipmaps( CommandBuffer & commandBuffer )const;
		/**
		*\~french
		*\brief
		*	Génère les mipmaps de la texture.
		*\~english
		*\brief
		*	Generates the texture mipmaps.
		*/
		void generateMipmaps( CommandPool const & commandPool
			, Queue const & queue )const;
		/**
		*\~english
		*\return
		*	The memory requirements for this buffer.
		*\~french
		*\return
		*	Les exigences mémoire pour ce tampon.
		*/
		VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\~french
		*\brief
		*	Crée une vue sur la texture.
		*\param[in] createInfo
		*	Les informations de création de la vue.
		*\~english
		*\brief
		*	Creates a view to the texture.
		*\param[in] createInfo
		*	The view creation informations.
		*/
		ImageView createView( VkImageViewCreateInfo createInfo )const;
		/**
		*\~french
		*\brief
		*	Crée une vue sur la texture.
		*\param[in] type
		*	Le type de texture de la vue.
		*\param[in] format
		*	Le format des pixels de la vue.
		*\param[in] baseMipLevel
		*	Le premier niveau de mipmap accessible à la vue.
		*\param[in] levelCount
		*	Le nombre de niveaux de mipmap (à partir de \p baseMipLevel) accessibles à la vue.
		*\param[in] baseArrayLayer
		*	La première couche de tableau accessible à la vue.
		*\param[in] layerCount
		*	Le nombre de couches de tableau (à partir de \p baseArrayLayer) accessibles à la vue.
		*\param[in] mapping
		*	Le mapping des composantes de couleur.
		*\~english
		*\brief
		*	Creates a view to the texture.
		*\param[in] type
		*	The view's texture type.
		*\param[in] format
		*	The view's pixels format.
		*\param[in] baseMipLevel
		*	The first mipmap level accessible to the view.
		*\param[in] levelCount
		*	The number of mipmap levels (starting from \p baseMipLevel) accessible to the view.
		*\param[in] baseArrayLayer
		*	The first array layer accessible to the view.
		*\param[in] layerCount
		*	The number of array layers (starting from \p baseArrayLayer) accessible to the view.
		*\param[in] mapping
		*	The colours component mapping.
		*/
		ImageView createView( VkImageViewType type
			, VkFormat format
			, uint32_t baseMipLevel = 0u
			, uint32_t levelCount = 1u
			, uint32_t baseArrayLayer = 0u
			, uint32_t layerCount = 1u
			, VkComponentMapping const & mapping = VkComponentMapping{} )const;
		/**
		*\name
		*	Getters.
		**/
		/**@{*/
		inline VkFormat getFormat()const noexcept
		{
			return m_createInfo->format;
		}

		inline uint32_t getLayerCount()const noexcept
		{
			return m_createInfo->arrayLayers;
		}

		inline VkImageCreateFlags getFlags()const noexcept
		{
			return m_createInfo->flags;
		}

		inline uint32_t getMipmapLevels()const noexcept
		{
			return m_createInfo->mipLevels;
		}

		inline VkExtent3D const & getDimensions()const noexcept
		{
			return m_createInfo->extent;
		}

		inline VkImageType getType()const
		{
			return m_createInfo->imageType;
		}

		inline VkImageTiling getTiling()const
		{
			return m_createInfo->tiling;
		}
		/**@}*/
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkImage.
		*\~english
		*\brief
		*	VkImage implicit cast operator.
		*/
		inline operator VkImage const & ()const
		{
			return m_internal;
		}

	private:
		void destroyView( VkImageView view )const;
		void destroyView( ImageView view )const;

	private:
		Device const * m_device{ nullptr };
		ImageCreateInfo m_createInfo{ 0u, VK_IMAGE_TYPE_2D, VK_FORMAT_UNDEFINED, { 1u, 1u, 1u }, 1u, 1u, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, 0u };
		VkImage m_internal{ VK_NULL_HANDLE };
		DeviceMemoryPtr m_storage;
		bool m_ownInternal{ true };
		mutable std::map< VkImageView, std::unique_ptr< VkImageViewCreateInfo > > m_views;
	};
}

#endif