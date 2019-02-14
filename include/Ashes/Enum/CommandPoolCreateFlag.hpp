/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_CommandPoolCreateFlag_HPP___
#define ___Ashes_CommandPoolCreateFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des utilisation de tampon de commandes.
	*/
	enum class CommandPoolCreateFlag
		: uint32_t
	{
		eTransient = 0x00000001,
		eResetCommandBuffer = 0x00000002,
	};
	Ashes_ImplementFlag( CommandPoolCreateFlag )
}

#endif
