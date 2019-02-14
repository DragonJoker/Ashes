/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PhysicalDeviceMemoryProperties_HPP___
#define ___Ashes_PhysicalDeviceMemoryProperties_HPP___
#pragma once

#include "Ashes/Miscellaneous/MemoryHeap.hpp"
#include "Ashes/Miscellaneous/MemoryType.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a physical device memory properties.
	*\~french
	*\brief
	*	Définit les propriétés mémoire d'un périphérique physique.
	*/
	struct PhysicalDeviceMemoryProperties
	{
		std::vector< MemoryType > memoryTypes;
		std::vector< MemoryHeap > memoryHeaps;
	};
}

#endif
