/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DescriptorPoolCreateFlag_HPP___
#define ___Ashes_DescriptorPoolCreateFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Bitmask specifying certain supported operations on a descriptor pool.
	*\~french
	*\brief
	*	Masque de bits définissant les opérations supportées sur un pool de descripteurs.
	*/
	enum class DescriptorPoolCreateFlag
		: uint32_t
	{
		eFreeDescriptorSet = 0x00000001,
	};
	Renderer_ImplementFlag( DescriptorPoolCreateFlag )
}

#endif
