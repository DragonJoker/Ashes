/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_CompositeAlphaFlag_HPP___
#define ___Ashes_CompositeAlphaFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Alpha composite flags.
	*\~french
	*\brief
	*	Indicateurs de composition alpha.
	*/
	enum class CompositeAlphaFlag
		: uint32_t
	{
		eOpaque = 0x00000001,
		ePreMultiplied = 0x00000002,
		ePostMultiplied = 0x00000004,
		eInherit = 0x00000008,
	};
	Renderer_ImplementFlag( CompositeAlphaFlag )
}

#endif
