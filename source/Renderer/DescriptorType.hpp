/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

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
	/**
	*\brief
	*	Convertit un renderer::DescriptorType en VkDescriptorType.
	*\param[in] type
	*	Le renderer::DescriptorType.
	*\return
	*	Le VkDescriptorType.
	*/
	VkDescriptorType convert( DescriptorType const & type );
}
