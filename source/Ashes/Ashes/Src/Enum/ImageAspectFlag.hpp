/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageAspectFlag_HPP___
#define ___Ashes_ImageAspectFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Masques de bits décrivant les aspects d'une image.
	*/
	enum class ImageAspectFlag
		: uint32_t
	{
		eColour = 0x00000001,
		eDepth = 0x00000002,
		eStencil = 0x00000004,
		eMetaData = 0x00000008,
	};
	Renderer_ImplementFlag( ImageAspectFlag )
	/**
	*\~french
	*\brief
	*	Récupère le masque d'aspects correspondant au ashes::Format donné.
	*\param[in] format
	*	Le ashes::Format.
	*\return
	*	Les aspects.
	*\~english
	*\brief
	*	Retrieves the aspects mask matching given ashes::Format.
	*\param[in] format
	*	The ashes::Format.
	*\return
	*	The aspects.
	*/
	inline ImageAspectFlags getAspectMask( Format format )noexcept
	{
		return isDepthStencilFormat( format )
			? ImageAspectFlag::eDepth | ImageAspectFlag::eStencil
			: ( isDepthFormat( format )
				? ImageAspectFlags{ ImageAspectFlag::eDepth }
				: ( isStencilFormat( format )
					? ImageAspectFlags{ ImageAspectFlag::eStencil }
					: ImageAspectFlags{ ImageAspectFlag::eColour } ) );
	}
}

#endif
