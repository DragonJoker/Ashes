/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class SwapchainKHR
		: public IcdObject
	{
	public:
		SwapchainKHR( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkSwapchainCreateInfoKHR createInfo );
		~SwapchainKHR();

		uint32_t getImageCount()const;
		VkImageArray getImages()const;
		VkResult acquireNextImage( uint64_t timeout
			, VkSemaphore semaphore
			, VkFence fence
			, uint32_t & imageIndex )const;

		VkResult present( uint32_t imageIndex )const;

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkAllocationCallbacks const * m_allocInfo;
		VkDevice m_device;
		VkSwapchainCreateInfoKHR m_createInfo;
		VkImage m_image;
		VkDeviceMemory m_deviceMemory;
		VkImageView m_view;
	};
}
