/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MemoryHeap_HPP___
#define ___Ashes_MemoryHeap_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a memory heap.
	*\~french
	*\brief
	*	Spécifie un tas mémoire.
	*/
	struct MemoryHeap
	{
		uint64_t size;
		MemoryHeapFlags flags;
	};
}

#endif
