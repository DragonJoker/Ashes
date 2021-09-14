/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SwapChainCreateInfo_HPP___
#define ___AshesPP_SwapChainCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct SwapChainCreateInfo
	{
		SwapChainCreateInfo( SwapChainCreateInfo const & ) = delete;
		SwapChainCreateInfo & operator=( SwapChainCreateInfo const & ) = delete;

		SwapChainCreateInfo( VkSwapchainCreateFlagsKHR pflags
			, VkSurfaceKHR psurface
			, uint32_t pminImageCount
			, VkFormat pimageFormat
			, VkColorSpaceKHR pimageColorSpace
			, VkExtent2D pimageExtent
			, uint32_t pimageArrayLayers
			, VkImageUsageFlags pimageUsage
			, VkSharingMode pimageSharingMode
			, UInt32Array pqueueFamilyIndices
			, VkSurfaceTransformFlagBitsKHR ppreTransform
			, VkCompositeAlphaFlagBitsKHR pcompositeAlpha
			, VkPresentModeKHR ppresentMode
			, VkBool32 pclipped
			, VkSwapchainKHR poldSwapchain )
			: queueFamilyIndices{ std::move( pqueueFamilyIndices ) }
			, vk
			{
				VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				nullptr,
				pflags,
				psurface,
				pminImageCount,
				pimageFormat,
				pimageColorSpace,
				pimageExtent,
				pimageArrayLayers,
				pimageUsage,
				pimageSharingMode,
				uint32_t( queueFamilyIndices.size() ),
				queueFamilyIndices.data(),
				ppreTransform,
				pcompositeAlpha,
				ppresentMode,
				pclipped,
				poldSwapchain,
			}
		{
		}
		
		SwapChainCreateInfo( SwapChainCreateInfo && rhs )noexcept
			: queueFamilyIndices{ std::move( rhs.queueFamilyIndices ) }
			, vk
			{
				VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				nullptr,
				rhs.vk.flags,
				rhs.vk.surface,
				rhs.vk.minImageCount,
				rhs.vk.imageFormat,
				rhs.vk.imageColorSpace,
				rhs.vk.imageExtent,
				rhs.vk.imageArrayLayers,
				rhs.vk.imageUsage,
				rhs.vk.imageSharingMode,
				uint32_t( queueFamilyIndices.size() ),
				queueFamilyIndices.data(),
				rhs.vk.preTransform,
				rhs.vk.compositeAlpha,
				rhs.vk.presentMode,
				rhs.vk.clipped,
				rhs.vk.oldSwapchain,
			}
		{
		}
		
		SwapChainCreateInfo & operator=( SwapChainCreateInfo && rhs )noexcept
		{
			queueFamilyIndices = std::move( rhs.queueFamilyIndices );
			vk =
			{
				VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				nullptr,
				rhs.vk.flags,
				rhs.vk.surface,
				rhs.vk.minImageCount,
				rhs.vk.imageFormat,
				rhs.vk.imageColorSpace,
				rhs.vk.imageExtent,
				rhs.vk.imageArrayLayers,
				rhs.vk.imageUsage,
				rhs.vk.imageSharingMode,
				uint32_t( queueFamilyIndices.size() ),
				queueFamilyIndices.data(),
				rhs.vk.preTransform,
				rhs.vk.compositeAlpha,
				rhs.vk.presentMode,
				rhs.vk.clipped,
				rhs.vk.oldSwapchain,
			};

			return *this;
		}

		inline operator VkSwapchainCreateInfoKHR const &()const
		{
			return vk;
		}

	private:
		UInt32Array queueFamilyIndices;
		VkSwapchainCreateInfoKHR vk;
	};
}

#endif
