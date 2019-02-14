/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DescriptorType_HPP___
#define ___Ashes_DescriptorType_HPP___
#pragma once

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Masques de bits décrivant les propriétés pour un type de mémoire.
	*\~english
	*\brief
	*	Bitmask specifying properties for a memory type.
	*/
	enum class DescriptorType
		: uint32_t
	{
		eSampler,
		eCombinedImageSampler,
		eSampledImage,
		eStorageImage,
		eUniformTexelBuffer,
		eStorageTexelBuffer,
		eUniformBuffer,
		eStorageBuffer,
		eUniformBufferDynamic,
		eStorageBufferDynamic,
		eInputAttachment,
	};
}

#endif
