/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_WriteDescriptorSet_HPP___
#define ___Renderer_WriteDescriptorSet_HPP___
#pragma once

#include "Buffer/BufferView.hpp"
#include "DescriptorImageInfo.hpp"
#include "DescriptorBufferInfo.hpp"

namespace renderer
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
