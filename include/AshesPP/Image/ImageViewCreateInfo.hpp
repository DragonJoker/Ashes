/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageViewCreateInfo_HPP___
#define ___Ashes_ImageViewCreateInfo_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

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
			: vk
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				nullptr,
				flags,
				image,
				viewType,
				format,
				std::move( components ),
				std::move( subresourceRange ),
			}
		{
		}

		inline operator VkImageViewCreateInfo const &()const
		{
			return vk;
		}

	private:
		VkImageViewCreateInfo vk;
	};
}

#endif
