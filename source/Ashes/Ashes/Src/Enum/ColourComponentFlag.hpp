/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ColourComponentFlag_HPP___
#define ___Ashes_ColourComponentFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class ColourComponentFlag
		: uint32_t
	{
		eR = 0x00000001,
		eG = 0x00000002,
		eB = 0x00000004,
		eA = 0x00000008,
	};
	Renderer_ImplementFlag( ColourComponentFlag )
}

#endif
