/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Image_HPP___
#define ___Ashes_Image_HPP___
#pragma once

#include "Ashes/Image/ComponentMapping.hpp"
#include "Ashes/Image/ImageCreateInfo.hpp"
#include "Ashes/Image/ImageViewCreateInfo.hpp"
#include "Ashes/Miscellaneous/DeviceMemory.hpp"

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
	protected:
		Image( Image const & ) = delete;
		Image( Image && rhs );
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] type
		*	Le type de texture.
		*\param[in] format
		*	Le format des données de la texture.
		*\param[in] dimensions
		*	Les dimensions de la texture.
		*\param[in] mipLevels
		*	Le nombre de niveaux de mipmaps.
		*\param[in] arrayLayers
		*	Le nombre de couches du tableau.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] type
		*	The texture's type.
		*\param[in] format
		*	The texture's data type.
		*\param[in] dimensions
		*	The texture's dimensions.
		*\param[in] mipLevels
		*	The mipmap levelx count.
		*\param[in] arrayLayers
		*	The array layers count.
		*/
		Image( Device const & device
			, ImageCreateFlags flags
			, ImageType type
			, Format format
			, Extent3D dimensions
			, uint32_t mipLevels
			, uint32_t arrayLayers );

	public:
		Image & operator=( Image const & ) = delete;
		Image & operator=( Image && rhs );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Image();
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
		*	Mappe la zone mémoire de l'image en RAM.
		*\param[in] offset
		*	L'offset en pixels de la zone mémoire.
		*\param[in] size
		*	La taille en pixels de la zone mémoire.
		*\param[in] subResourceLayers
		*	Les couches et niveaux de la zone mémoire.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*\~english
		*\brief
		*	Maps the image's memory area in RAM.
		*\param[in] offset
		*	The memory area starting offset, in pixels.
		*\param[in] size
		*	The memory area size, in pixels.
		*\param[in] subResourceLayers
		*	The memory area levels and layers.
		*\param[in] flags
		*	The memory mapping flags.
		*\return
		*	\p nullptr if the mapping failed.
		*/
		uint8_t * lock( SubresourceLayout const & subResourceLayout
			, MemoryMapFlags flags )const;
		/**
		*\~english
		*\brief
		*	Invalidates the image content.
		*\param[in] offset
		*	The memory area starting offset, in pixels.
		*\param[in] size
		*	The memory area size, in pixels.
		*\param[in] subResourceLayers
		*	The memory area levels and layers.
		*\~french
		*\brief
		*	Invalide le contenu de l'image.
		*\param[in] offset
		*	L'offset en pixels de la zone mémoire.
		*\param[in] size
		*	La taille en pixels de la zone mémoire.
		*\param[in] subResourceLayers
		*	Les couches et niveaux de la zone mémoire.
		*/
		void invalidate( SubresourceLayout const & subResourceLayout )const;
		/**
		*\~english
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The memory area starting offset, in pixels.
		*\param[in] size
		*	The memory area size, in pixels.
		*\param[in] subResourceLayers
		*	The memory area levels and layers.
		*\~french
		*\brief
		*	Met à jour la VRAM.
		*\param[in] offset
		*	L'offset en pixels de la zone mémoire.
		*\param[in] size
		*	La taille en pixels de la zone mémoire.
		*\param[in] subResourceLayers
		*	Les couches et niveaux de la zone mémoire.
		*/
		void flush( SubresourceLayout const & subResourceLayout )const;
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
		virtual void generateMipmaps( CommandBuffer & commandBuffer )const;
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
		virtual MemoryRequirements getMemoryRequirements()const = 0;
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
		virtual ImageViewPtr createView( ImageViewCreateInfo const & createInfo )const = 0;
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
		ImageViewPtr createView( ImageViewType type
			, Format format
			, uint32_t baseMipLevel = 0u
			, uint32_t levelCount = 1u
			, uint32_t baseArrayLayer = 0u
			, uint32_t layerCount = 1u
			, ComponentMapping const & mapping = ComponentMapping{} )const;
		/**
		*\~french
		*\return
		*	Le format des pixels de la texture.
		*\~english
		*\return
		*	The texture pixel format.
		*/
		inline Format getFormat()const noexcept
		{
			return m_format;
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
			return m_arrayLayers;
		}
		/**
		*\~french
		*\return
		*	Les indicateurs de création.
		*\~english
		*\return
		*	The creation flags.
		*/
		inline ImageCreateFlags getFlags()const noexcept
		{
			return m_flags;
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
			return m_mipLevels;
		}
		/**
		*\~french
		*\return
		*	Les dimensions de la texture.
		*\~english
		*\return
		*	The texture dimensions.
		*/
		inline Extent3D const & getDimensions()const noexcept
		{
			return m_dimensions;
		}
		/**
		*\~french
		*\return
		*	Le type de texture.
		*\~english
		*\return
		*	The texture type.
		*/
		inline ImageType getType()const
		{
			return m_imageType;
		}

	private:
		virtual void doBindMemory() = 0;

	protected:
		Device const & m_device;
		ImageCreateFlags m_flags;
		ImageType m_imageType;
		Format m_format;
		Extent3D m_dimensions;
		uint32_t m_mipLevels;
		uint32_t m_arrayLayers;
		DeviceMemoryPtr m_storage;
	};
}

#endif
