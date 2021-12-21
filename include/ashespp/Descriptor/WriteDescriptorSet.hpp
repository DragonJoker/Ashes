/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_WriteDescriptorSet_HPP___
#define ___AshesPP_WriteDescriptorSet_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct WriteDescriptorSet
	{
		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, uint32_t descriptorCount
			, VkDescriptorType descriptorType )
			: vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, descriptorCount, descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
		}

		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, VkDescriptorType descriptorType
			, VkDescriptorImageInfoArray imageInfos )
			: imageInfo{ std::move( imageInfos ) }
			, vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, uint32_t( this->imageInfo.size() ), descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
		}

		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, VkDescriptorType descriptorType
			, VkDescriptorBufferInfoArray bufferInfos )
			: bufferInfo{ std::move( bufferInfos ) }
			, vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, uint32_t( this->bufferInfo.size() ), descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
		}

		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, VkDescriptorType descriptorType
			, VkDescriptorBufferInfoArray bufferInfos
			, VkBufferViewArray viewInfos )
			: bufferInfo{ std::move( bufferInfos ) }
			, texelBufferView{ std::move( viewInfos ) }
			, vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, uint32_t( this->bufferInfo.size() ), descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
			assert( bufferInfo.size() == texelBufferView.size() );
		}

		WriteDescriptorSet( VkDescriptorSet set
			, uint32_t dstBinding
			, uint32_t dstArrayElement
			, uint32_t descriptorCount
			, VkDescriptorType descriptorType
			, VkDescriptorImageInfo const * pimageInfo
			, VkDescriptorBufferInfo const * pbufferInfo
			, VkBufferView const * ptexelBufferView )
			: vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, set, dstBinding, dstArrayElement, descriptorCount, descriptorType, pimageInfo, pbufferInfo, ptexelBufferView }
			, needsUpdate{ false }
		{
		}

		void update( VkDescriptorSet descriptorSet )const
		{
			if ( needsUpdate )
			{
				vk.dstSet = descriptorSet;
				vk.pImageInfo = imageInfo.data();
				vk.pBufferInfo = bufferInfo.data();
				vk.pTexelBufferView = texelBufferView.data();
			}
		}

		operator VkWriteDescriptorSet const &()const
		{
			return vk;
		}

		inline VkWriteDescriptorSet const * operator->()const
		{
			return &vk;
		}

		inline VkWriteDescriptorSet * operator->()
		{
			return &vk;
		}

		VkDescriptorImageInfoArray imageInfo;
		VkDescriptorBufferInfoArray bufferInfo;
		VkBufferViewArray texelBufferView;

	private:
		mutable VkWriteDescriptorSet vk{};
		bool needsUpdate;
	};
}

#endif
