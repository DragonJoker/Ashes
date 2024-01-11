/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageViewCreateInfo_HPP___
#define ___AshesPP_ImageViewCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct ImageViewCreateInfo
	{
		ImageViewCreateInfo( VkImageViewCreateFlags flags
			, VkImage image
			, VkImageViewType viewType
			, VkFormat format
			, VkComponentMapping components
			, VkImageSubresourceRange subresourceRange )
			: vk{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
				, nullptr
				, flags
				, image
				, viewType
				, format
				, std::move( components )
				, std::move( subresourceRange ) }
		{
		}

		explicit ImageViewCreateInfo( VkImageViewCreateInfo const & createInfo )
			: vk{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
				, createInfo.pNext
				, createInfo.flags
				, createInfo.image
				, createInfo.viewType
				, createInfo.format
				, createInfo.components
				, createInfo.subresourceRange }
		{
		}

		operator VkImageViewCreateInfo const &()const noexcept
		{
			return vk;
		}

		VkImageViewCreateInfo const * operator->()const noexcept
		{
			return &vk;
		}

		VkImageViewCreateInfo * operator->()noexcept
		{
			return &vk;
		}

	private:
		VkImageViewCreateInfo vk;
	};
}

#endif
