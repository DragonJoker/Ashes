/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_CommandBufferBeginInfo_HPP___
#define ___Ashes_CommandBufferBeginInfo_HPP___
#pragma once

#include "Enum/CommandBufferUsageFlag.hpp"
#include "Command/CommandBufferInheritanceInfo.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a command buffer begin operation.
	*\~french
	*\brief
	*	Définit le début d'un tampon de commandes.
	*/
	struct CommandBufferBeginInfo
	{
		/**
		*\~english
		*\brief
		*	The usage flags for the command buffer.
		*\~french
		*\brief
		*	Les indicateurs de type de charge qui sera affectée au tampon.
		*/
		CommandBufferUsageFlags flags;
		/**
		*\~english
		*\brief
		*	The inheritance informations.
		*\~french
		*\brief
		*	Les informations d'héritage.
		*/
		Optional< CommandBufferInheritanceInfo > inheritanceInfo;
	};
}

#endif
