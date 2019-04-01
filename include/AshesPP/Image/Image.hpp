/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Texture_HPP___
#define ___AshesPP_Texture_HPP___
#pragma once

#include "AshesPP/Miscellaneous/DeviceMemory.hpp"

namespace ashespp
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
		Image( Image && rhs );
		Image & operator=( Image const & ) = delete;
		Image & operator=( Image && rhs );

	public:
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
			, VkImageCreateInfo createInfo );
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
		ImageViewPtr createView( VkImageViewCreateInfo const & createInfo )const;
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
		ImageViewPtr createView( VkImageViewType type
			, VkFormat format
			, uint32_t baseMipLevel = 0u
			, uint32_t levelCount = 1u
			, uint32_t baseArrayLayer = 0u
			, uint32_t layerCount = 1u
			, VkComponentMapping const & mapping = VkComponentMapping{} )const;
		/**
		*\~french
		*\return
		*	Le format des pixels de la texture.
		*\~english
		*\return
		*	The texture pixel format.
		*/
		inline VkFormat getFormat()const noexcept
		{
			return m_createInfo.format;
		}
		/**
		*\~french
		*\return
		*	Le nombre de couches.
		*\~english
		*\return
		*	The layers count.
		*/
		inline uint32_t getLayerCount()const noexcept
		{
			return m_createInfo.arrayLayers;
		}
		/**
		*\~french
		*\return
		*	Les indicateurs de création.
		*\~english
		*\return
		*	The creation flags.
		*/
		inline VkImageCreateFlags getFlags()const noexcept
		{
			return m_createInfo.flags;
		}
		/**
		*\~french
		*\return
		*	Le nombre de niveaux de mipmaps.
		*\~english
		*\return
		*	The mipmap levels count.
		*/
		inline uint32_t getMipmapLevels()const noexcept
		{
			return m_createInfo.mipLevels;
		}
		/**
		*\~french
		*\return
		*	Les dimensions de la texture.
		*\~english
		*\return
		*	The texture dimensions.
		*/
		inline VkExtent3D const & getDimensions()const noexcept
		{
			return m_createInfo.extent;
		}
		/**
		*\~french
		*\return
		*	Le type de texture.
		*\~english
		*\return
		*	The texture type.
		*/
		inline VkImageType getType()const
		{
			return m_createInfo.imageType;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkImage.
		*\~english
		*\brief
		*	VkImage implicit cast operator.
		*/
		inline operator VkImage const & ( )const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkImageCreateInfo m_createInfo;
		bool m_ownInternal;
		VkImage m_internal{ VK_NULL_HANDLE };
		DeviceMemoryPtr m_storage;
	};
}

#endif
