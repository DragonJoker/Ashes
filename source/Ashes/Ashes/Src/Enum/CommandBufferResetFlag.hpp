/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_CommandBufferResetFlag_HPP___
#define ___Ashes_CommandBufferResetFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des indicateurs de réinitalisation de tampon de commandes.
	*/
	enum class CommandBufferResetFlag
		: uint32_t
	{
		eReleaseResources = 0x00000001,
	};
	Renderer_ImplementFlag( CommandBufferResetFlag )
}

#endif
