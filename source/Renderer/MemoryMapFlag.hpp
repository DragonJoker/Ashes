/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*@~french
	*@brief
	*	Masques de bits décrivant les propriétés pour un type de mémoire.
	*@~english
	*@brief
	*	Bimask specifying properties for a memory type.
	*/
	enum class MemoryMapFlag
		: uint32_t
	{
	};
	VkLib_ImplementFlag( MemoryMapFlag )
	/**
	*\brief
	*	Convertit un renderer::MemoryMapFlags en VkMemoryMapFlags.
	*\param[in] format
	*	Le renderer::MemoryMapFlags.
	*\return
	*	Le VkMemoryMapFlags.
	*/
	VkMemoryMapFlags convert( MemoryMapFlags flags );
}
