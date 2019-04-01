/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___AshesPP_Format_HPP___
#define ___AshesPP_Format_HPP___
#pragma once

#include <string>

namespace ashespp
{
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
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de profondeur et stencil.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in depth and stencil buffers.
	*\param[in] format
	*	The ashes::VkFormat.
	*\return
	*	\p true if it is usable in depth and stencil buffers.
	*/
	bool isDepthStencilFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de stencil.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in stencil buffers.
	*\param[in] format
	*	The ashes::VkFormat.
	*\return
	*	\p true if it is usable in stencil buffers.
	*/
	bool isStencilFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de profondeur.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in depth buffers.
	*\param[in] format
	*	The ashes::VkFormat.
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
	bool isCompressedFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de profondeur et/ou stencil.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in depth and/or stencil buffers.
	*\param[in] format
	*	The ashes::VkFormat.
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
	VkImageAspectFlags getAspectMask( VkFormat format )noexcept;
}

#endif
