/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_WriteDescriptorSet_HPP___
#define ___Ashes_WriteDescriptorSet_HPP___
#pragma once

#include "AshesRenderer/Buffer/BufferView.hpp"
#include "AshesRenderer/Descriptor/DescriptorImageInfo.hpp"
#include "AshesRenderer/Descriptor/DescriptorBufferInfo.hpp"

namespace ashes
{
	struct WriteDescriptorSet
	{
		uint32_t dstBinding;
		uint32_t dstArrayElement;
		uint32_t descriptorCount;
		DescriptorType descriptorType;
		std::vector< DescriptorImageInfo > imageInfo;
		std::vector< DescriptorBufferInfo > bufferInfo;
		std::vector< BufferViewCRef > texelBufferView;
	};
}

#endif
