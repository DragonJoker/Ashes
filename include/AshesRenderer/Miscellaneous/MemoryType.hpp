/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MemoryType_HPP___
#define ___Ashes_MemoryType_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a memory type.
	*\~french
	*\brief
	*	Définit un type de mémoire.
	*/
	struct MemoryType
	{
		MemoryPropertyFlags propertyFlags;
		uint32_t heapIndex;
	};
}

#endif
