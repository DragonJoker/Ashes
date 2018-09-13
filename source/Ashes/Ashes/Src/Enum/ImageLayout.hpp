/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageLayout_HPP___
#define ___Ashes_ImageLayout_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des layouts d'image.
	*/
	enum class ImageLayout
	{
		eUndefined = 0,
		eGeneral = 1,
		eColourAttachmentOptimal = 2,
		eDepthStencilAttachmentOptimal = 3,
		eDepthStencilReadOnlyOptimal = 4,
		eShaderReadOnlyOptimal = 5,
		eTransferSrcOptimal = 6,
		eTransferDstOptimal = 7,
		ePreinitialised = 8,
		Ashes_EnumBounds( eUndefined ),
		ePresentSrc = 1000001002,
	};
}

#endif
