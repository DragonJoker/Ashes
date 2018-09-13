/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_FenceCreateFlag_HPP___
#define ___Ashes_FenceCreateFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class FenceCreateFlag
		: uint32_t
	{
		eSignaled = 0x00000001,
	};
	Renderer_ImplementFlag( FenceCreateFlag )
}

#endif
