/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SamplerCreateInfo_HPP___
#define ___AshesPP_SamplerCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct SamplerCreateInfo
	{
		SamplerCreateInfo( VkSamplerCreateInfo && info )
			: vk{ std::move( info ) }
		{
		}

		SamplerCreateInfo( VkSamplerCreateFlags flags
			, VkFilter magFilter
			, VkFilter minFilter
			, VkSamplerMipmapMode mipmapMode
			, VkSamplerAddressMode addressModeU
			, VkSamplerAddressMode addressModeV
			, VkSamplerAddressMode addressModeW
			, float mipLodBias
			, VkBool32 anisotropyEnable
			, float maxAnisotropy
			, VkBool32 compareEnable
			, VkCompareOp compareOp
			, float minLod = -1000.0f
			, float maxLod = 1000.0f
			, VkBorderColor borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK
			, VkBool32 unnormalizedCoordinates = VK_FALSE )
			: vk{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO
				, nullptr
				, flags
				, magFilter
				, minFilter
				, mipmapMode
				, addressModeU
				, addressModeV
				, addressModeW
				, mipLodBias
				, anisotropyEnable
				, maxAnisotropy
				, compareEnable
				, compareOp
				, minLod
				, maxLod
				, borderColor
				, unnormalizedCoordinates }
		{
		}

		operator VkSamplerCreateInfo const &()const
		{
			return vk;
		}

		VkSamplerCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkSamplerCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		VkSamplerCreateInfo vk;
	};
}

#endif
