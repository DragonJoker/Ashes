/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_MemoryMapFlag_HPP___
#define ___Renderer_MemoryMapFlag_HPP___
#pragma once

#include <Utils/FlagCombination.hpp>

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Masques de bits décrivant les propriétés pour un type de mémoire.
	*\~english
	*\brief
	*	Bimask specifying properties for a memory type.
	*/
	enum class MemoryMapFlag
		: uint32_t
	{
	};
	Utils_ImplementFlag( MemoryMapFlag )
}

#endif
