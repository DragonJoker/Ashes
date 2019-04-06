/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SwapChainCreateInfo_HPP___
#define ___Ashes_SwapChainCreateInfo_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	struct SwapChainCreateInfo
	{
		SwapChainCreateInfo( SwapChainCreateInfo const & ) = delete;
		SwapChainCreateInfo & operator=( SwapChainCreateInfo const & ) = delete;

		SwapChainCreateInfo( VkSwapchainCreateFlagsKHR flags
			, VkSurfaceKHR surface
			, uint32_t minImageCount
			, VkFormat imageFormat
			, VkColorSpaceKHR imageColorSpace
			, VkExtent2D imageExtent
			, uint32_t imageArrayLayers
			, VkImageUsageFlags imageUsage
			, VkSharingMode imageSharingMode
			, UInt32Array queueFamilyIndices
			, VkSurfaceTransformFlagBitsKHR preTransform
			, VkCompositeAlphaFlagBitsKHR compositeAlpha
			, VkPresentModeKHR presentMode
			, VkBool32 clipped
			, VkSwapchainKHR oldSwapchain )
			: queueFamilyIndices{ std::move( queueFamilyIndices ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				nullptr,
				flags,
				surface,
				minImageCount,
				imageFormat,
				imageColorSpace,
				imageExtent,
				imageArrayLayers,
				imageUsage,
				imageSharingMode,
				uint32_t( this->queueFamilyIndices.size() ),
				this->queueFamilyIndices.data(),
				preTransform,
				compositeAlpha,
				presentMode,
				clipped,
				oldSwapchain,
			}
		{
		}
		
		SwapChainCreateInfo( SwapChainCreateInfo && rhs )
			: queueFamilyIndices{ std::move( rhs.queueFamilyIndices ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
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
				uint32_t( this->queueFamilyIndices.size() ),
				this->queueFamilyIndices.data(),
				rhs.vk.preTransform,
				rhs.vk.compositeAlpha,
				rhs.vk.presentMode,
				rhs.vk.clipped,
				rhs.vk.oldSwapchain,
			}
		{
		}
		
		SwapChainCreateInfo & operator=( SwapChainCreateInfo && rhs )
		{
			queueFamilyIndices = std::move( rhs.queueFamilyIndices );
			vk =
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
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
				uint32_t( this->queueFamilyIndices.size() ),
				this->queueFamilyIndices.data(),
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
