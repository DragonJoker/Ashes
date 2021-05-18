/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageCreateInfo_HPP___
#define ___AshesPP_ImageCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct ImageCreateInfo
	{
		ImageCreateInfo( VkImageCreateFlags flags
			, VkImageType imageType
			, VkFormat format
			, VkExtent3D extent
			, uint32_t mipLevels
			, uint32_t arrayLayers
			, VkSampleCountFlagBits samples
			, VkImageTiling tiling
			, VkImageUsageFlags usage
			, VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE
			, UInt32Array queueFamilyIndices = {}
			, VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED )
			: queueFamilyIndices{ std::move( queueFamilyIndices ) }
			, vk{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
				, nullptr
				, flags
				, imageType
				, format
				, std::move( extent )
				, mipLevels
				, arrayLayers
				, samples
				, tiling
				, usage
				, sharingMode
				, uint32_t( this->queueFamilyIndices.size() )
				, this->queueFamilyIndices.data()
				, initialLayout }
		{
		}

		ImageCreateInfo( VkImageCreateInfo createInfo )
			: ImageCreateInfo{ createInfo.flags
				, createInfo.imageType
				, createInfo.format
				, std::move( createInfo.extent )
				, createInfo.mipLevels
				, createInfo.arrayLayers
				, createInfo.samples
				, createInfo.tiling
				, createInfo.usage
				, createInfo.sharingMode
				, { createInfo.pQueueFamilyIndices , createInfo.pQueueFamilyIndices + createInfo.queueFamilyIndexCount }
				, createInfo.initialLayout }
		{
		}

		inline operator VkImageCreateInfo const &()const
		{
			return vk;
		}

		inline VkImageCreateInfo const * operator->()const
		{
			return &vk;
		}

		inline VkImageCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		UInt32Array queueFamilyIndices;
		VkImageCreateInfo vk;
	};
}

#endif
