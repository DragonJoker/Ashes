/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_InstanceCreateFlag_HPP___
#define ___Ashes_InstanceCreateFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Instance creation flags.
	*\~french
	*\brief
	*	Indicateurs de création d'une instance.
	*/
	enum class InstanceCreateFlag
		: uint32_t
	{
		eNone,
	};
	Ashes_ImplementFlag( InstanceCreateFlag )
}

#endif
