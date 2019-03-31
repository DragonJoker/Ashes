/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_BufferCopy_HPP___
#define ___Ashes_BufferCopy_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a buffer copy operation.
	*\~french
	*\brief
	*	Définit une opération de copie de tampon.
	*/
	struct BufferCopy
	{
		/**
		*\~english
		*\brief
		*	The starting offset in bytes from the start of \p srcBuffer.
		*\~french
		*\brief
		*	Le décalage de départ en octets, depuis le début de \p srcBuffer.
		*/
		DeviceSize srcOffset;
		/**
		*\~english
		*\brief
		*	The starting offset in bytes from the start of \p dstBuffer.
		*\~french
		*\brief
		*	Le décalage de départ en octets, depuis le début de \p dstBuffer.
		*/
		DeviceSize dstOffset;
		/**
		*\~english
		*\brief
		*	The number of bytes to copy.
		*\~french
		*\brief
		*	Le nombre d'octets à copier.
		*/
		DeviceSize size;
	};
}

#endif
