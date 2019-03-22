/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MemoryAllocateInfo_HPP___
#define ___Ashes_MemoryAllocateInfo_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a memory allocation.
	*\~french
	*\brief
	*	Définit une allocation de mémoire.
	*/
	struct MemoryAllocateInfo
	{
		DeviceSize allocationSize;
		uint32_t memoryTypeIndex;
	};
}

#endif
