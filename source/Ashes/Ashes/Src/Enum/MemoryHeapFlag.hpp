/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MemoryHeapFlag_HPP___
#define ___Ashes_MemoryHeapFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Memory heap types enumeration.
	*\~french
	*\brief
	*	Enumération des types de pile mémoire.
	*/
	enum class MemoryHeapFlag
		: uint32_t
	{
		eDeviceLocal = 0x00000001,
	};
	Ashes_ImplementFlag( MemoryHeapFlag )
}

#endif
