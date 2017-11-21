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
	enum class BufferTarget
		: uint32_t
	{
		eTransferSrc = 0x00000001,
		eTransferDst = 0x00000002,
		eUniformTexelBuffer = 0x00000004,
		eStorageTexelBuffer = 0x00000008,
		eUniformBuffer = 0x00000010,
		eStorageBuffer = 0x00000020,
		eIndexBuffer = 0x00000040,
		eVertexBuffer = 0x00000080,
		eIndirectBuffer = 0x00000100,
	};
	VkLib_ImplementFlag( BufferTarget )
	/**
	*\brief
	*	Convertit un renderer::BufferTargets en VkBufferUsageFlags.
	*\param[in] targets
	*	Le renderer::BufferTargets.
	*\return
	*	Le VkBufferUsageFlags.
	*/
	VkBufferUsageFlags convert( BufferTargets targets );
}
