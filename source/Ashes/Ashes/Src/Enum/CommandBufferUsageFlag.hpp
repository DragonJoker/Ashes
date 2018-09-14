/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_CommandBufferUsageFlag_HPP___
#define ___Ashes_CommandBufferUsageFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des utilisation de tampon de commandes.
	*/
	enum class CommandBufferUsageFlag
		: uint32_t
	{
		eOneTimeSubmit = 0x00000001,
		eRenderPassContinue = 0x00000002,
		eSimultaneousUse = 0x00000004,
	};
	Ashes_ImplementFlag( CommandBufferUsageFlag )
}

#endif
