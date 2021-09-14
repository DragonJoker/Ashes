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
		ImageCreateInfo( VkImageCreateFlags pflags
			, VkImageType pimageType
			, VkFormat pformat
			, VkExtent3D pextent
			, uint32_t pmipLevels
			, uint32_t parrayLayers
			, VkSampleCountFlagBits psamples
			, VkImageTiling ptiling
			, VkImageUsageFlags pusage
			, VkSharingMode psharingMode = VK_SHARING_MODE_EXCLUSIVE
			, UInt32Array pqueueFamilyIndices = {}
			, VkImageLayout pinitialLayout = VK_IMAGE_LAYOUT_UNDEFINED )
			: queueFamilyIndices{ std::move( pqueueFamilyIndices ) }
			, vk{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
				, nullptr
				, pflags
				, pimageType
				, pformat
				, std::move( pextent )
				, pmipLevels
				, parrayLayers
				, psamples
				, ptiling
				, pusage
				, psharingMode
				, uint32_t( queueFamilyIndices.size() )
				, queueFamilyIndices.data()
				, pinitialLayout }
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
				, { createInfo.pQueueFamilyIndices, createInfo.pQueueFamilyIndices + createInfo.queueFamilyIndexCount }
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
