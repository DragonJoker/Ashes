/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <ashes/ashes.h>

#include <cstdint>
#include <string>
#include <algorithm>

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Computes an aligned size.
	*\param[in] size
	*	The unaligned size.
	*\param[in] align
	*	The alignment value.
	*\return
	*	The aligned size.
	*\~french
	*\brief
	*	Calcule une taille alignée.
	*\param[in] size
	*	La taille non alignée.
	*\param[in] align
	*	La valeur d'alignement.
	*\return
	*	La taille alignée.
	*/
	inline VkDeviceSize getAlignedSize( VkDeviceSize size, VkDeviceSize align )
	{
		VkDeviceSize result = 0u;

		while ( size > align )
		{
			size -= align;
			result += align;
		}

		return result + align;
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given non compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels non compressé donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getSize( VkFormat format )noexcept;
	/**
	*\return
	*	le nombre de composantes du format donné.
	*\param[in] format
	*	Le VkFormat.
	*/
	uint32_t getCount( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given BC compressed pixel format, for a 4x4 pixels block.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels compressé BC donné, pour un bloc de 4x4 pixels.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getBCSize( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given ETC2 compressed pixel format, for a 4x4 pixels block.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels compressé ETC2 donné, pour un bloc de 4x4 pixels.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getETC2Size( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given EAC compressed pixel format, for a 4x4 pixels block.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels compressé EAC donné, pour un bloc de 4x4 pixels.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getEACSize( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le VkFormat donné est un format utilisable pour les tampons de profondeur et stencil.
	*\param[in] format
	*	Le VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given VkFormat is usable in depth and stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth and stencil buffers.
	*/
	bool isDepthStencilFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le VkFormat donné est un format utilisable pour les tampons de stencil.
	*\param[in] format
	*	Le VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given VkFormat is usable in stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in stencil buffers.
	*/
	bool isStencilFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le VkFormat donné est un format utilisable pour les tampons de profondeur.
	*\param[in] format
	*	Le VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given VkFormat is usable in depth buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth buffers.
	*/
	bool isDepthFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a BC compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé BC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isBCFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a ETC2 compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé ETC2.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isETC2Format( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a EAC compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé EAC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isEACFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a ASTC compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé ASTC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isASTCFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a YCBCR pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format YCBCR.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isYCBCRFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a PVRTC pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format PVRTC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isPVRTCFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé.
	*\param[in] format
	*	Le format de pixel.
	*/
	inline bool isCompressedFormat( VkFormat format )noexcept
	{
		return isBCFormat( format )
			|| isEACFormat( format )
			|| isETC2Format( format )
			|| isASTCFormat( format )
			|| isYCBCRFormat( format )
			|| isPVRTCFormat( format );
	}
	/**
	*\~french
	*\brief
	*	Dit si le VkFormat donné est un format utilisable pour les tampons de profondeur et/ou stencil.
	*\param[in] format
	*	Le VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given VkFormat is usable in depth and/or stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth and/or stencil buffers.
	*/
	inline bool isDepthOrStencilFormat( VkFormat format )noexcept
	{
		return isDepthStencilFormat( format )
			|| isStencilFormat( format )
			|| isDepthFormat( format );
	}
	/**
	*\~english
	*\brief
	*	Gets the name of the given element.
	*\param[in] value
	*	The element.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom de l'élément donné.
	*\param[in] value
	*	L'élément.
	*\return
	*	Le nom.
	*/
	std::string getName( VkFormat value );
	/**
	*\~english
	*\brief
	*	Retrieves the minimal VkExtent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The VkExtent2D.
	*\~french
	*\brief
	*	Donne l'VkExtent2D minimales pour le format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	L'VkExtent2D.
	*/
	VkExtent2D getMinimalExtent2D( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*\~french
	*\brief
	*	Vérifie que les dimensiosn données sont compatibles avec le format donné.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	\p true si les dimensiosn sont compatibles avec le format.
	*/
	inline bool checkExtent( VkFormat format, VkExtent2D const & extent )
	{
		auto minimal = getMinimalExtent2D( format );
		return extent.width >= minimal.width
			&& extent.height >= minimal.height;
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getSize( VkExtent2D const & extent
		, VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the minimal VkExtent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The VkExtent2D.
	*\~french
	*\brief
	*	Donne l'VkExtent2D minimales pour le format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	L'VkExtent2D.
	*/
	inline VkExtent3D getMinimalExtent3D( VkFormat format )noexcept
	{
		auto const minimal = getMinimalExtent2D( format );
		return VkExtent3D{ minimal.width, minimal.height, 1 };
	}
	/**
	*\~english
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*\~french
	*\brief
	*	Vérifie que les dimensiosn données sont compatibles avec le format donné.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	\p true si les dimensions sont compatibles avec le format.
	*/
	inline bool checkExtent( VkFormat format, VkExtent3D const & extent )
	{
		return checkExtent( format, VkExtent2D{ extent.width, extent.height } );
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	inline uint32_t getSize( VkExtent3D const & extent
		, VkFormat format )noexcept
	{
		return getSize( VkExtent2D{ extent.width, extent.height }, format )
			* std::max( 1u, extent.depth );
	}
	/**
	*\~french
	*\brief
	*	Récupère le masque d'aspects correspondant au VkFormat donné.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Les aspects.
	*\~english
	*\brief
	*	Retrieves the aspects mask matching given VkFormat.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	The aspects.
	*/
	inline VkImageAspectFlags getAspectMask( VkFormat format )noexcept
	{
		return isDepthStencilFormat( format )
			? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
			: ( isDepthFormat( format )
				? VK_IMAGE_ASPECT_DEPTH_BIT
				: ( isStencilFormat( format )
					? VK_IMAGE_ASPECT_STENCIL_BIT
					: VK_IMAGE_ASPECT_COLOR_BIT ) );
	}
	/**
	*\~french
	*\brief
	*	Récupère une dimension réelle du niveau de mipmap donné.
	*\param[in] extent
	*	La dimension du niveau 0.
	*\param[in] mipLevel
	*	Le niveau de mipmap.
	*\return
	*	La dimension du niveau de mipmap.
	*\~english
	*\brief
	*	Retrieves the real extent for the given mipmap level.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level.
	*\return
	*	The mipmap level extent.
	*/
	template< typename T >
	inline T getSubresourceDimension( T const & extent
		, uint32_t mipLevel )
	{
		return extent >> mipLevel;
	}
	/**
	*\~french
	*\brief
	*	Récupère les dimensions réelles du niveau de mipmap donné.
	*\param[in] extent
	*	Les dimensions du niveau 0.
	*\param[in] mipLevel
	*	Le niveau de mipmap.
	*\return
	*	Les dimensions.
	*\~english
	*\brief
	*	Retrieves the real extent for the given mipmap level.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level.
	*\return
	*	The dimensions.
	*/
	inline VkExtent3D getSubresourceDimensions( VkExtent3D const & extent
		, uint32_t mipLevel )
	{
		return
		{
			getSubresourceDimension( extent.width, mipLevel ),
			getSubresourceDimension( extent.height, mipLevel ),
			extent.depth
		};
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	inline uint32_t getSize( VkExtent3D const & extent
		, VkFormat format
		, uint32_t mipLevel )noexcept
	{
		return getSize( getSubresourceDimensions( extent, mipLevel ), format );
	}
	/**
	*\~french
	*\brief
	*	Récupère les dimensions réelles du niveau de mipmap donné.
	*\param[in] extent
	*	Les dimensions du niveau 0.
	*\param[in] mipLevel
	*	Le niveau de mipmap.
	*\return
	*	Les dimensions.
	*\~english
	*\brief
	*	Retrieves the real extent for the given mipmap level.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level.
	*\return
	*	The dimensions.
	*/
	inline VkExtent3D getSubresourceDimensions( VkExtent2D const & extent
		, uint32_t mipLevel )
	{
		return
		{
			getSubresourceDimension( extent.width, mipLevel ),
			getSubresourceDimension( extent.height, mipLevel ),
		};
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	inline uint32_t getSize( VkExtent2D const & extent
		, VkFormat format
		, uint32_t mipLevel )noexcept
	{
		return getSize( getSubresourceDimensions( extent, mipLevel ), format );
	}
}
