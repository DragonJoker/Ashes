/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DeviceCreateFlag_HPP___
#define ___Ashes_DeviceCreateFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Logical device creation flags.
	*\~french
	*\brief
	*	Indicateurs de création d'un périphérique logique.
	*/
	enum class DeviceCreateFlag
		: uint32_t
	{
		eNone,
	};
	Ashes_ImplementFlag( DeviceCreateFlag )
}

#endif
