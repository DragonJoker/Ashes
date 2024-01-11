/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SwapChain_HPP___
#define ___AshesPP_SwapChain_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Class holding the concept of a swapchain.
	*/
	class SwapChain
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructor.
		*/
		SwapChain( Device const & device
			, VkSwapchainCreateInfoKHR createInfo );
		/**
		*\brief
		*	Constructor.
		*/
		SwapChain( Device const & device
			, std::string const & debugName
			, VkSwapchainCreateInfoKHR createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~SwapChain()noexcept;
		/**
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] semaphore
		*	\p nullptr or a semaphore to signal.
		*\param[in] fence
		*	\p nullptr or a fence to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Semaphore const * semaphore
			, Fence const * fence
			, uint32_t & imageIndex )const;
		/**
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] semaphore
		*	A semaphore to signal.
		*\param[in] fence
		*	A fence to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Semaphore const & semaphore
			, Fence const & fence
			, uint32_t & imageIndex )const;
		/**
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] fence
		*	A fence to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Fence const & fence
			, uint32_t & imageIndex )const;
		/**
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] semaphore
		*	\p nullptr or a semaphore to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Semaphore const & semaphore
			, uint32_t & imageIndex )const;
		/**
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, uint32_t & imageIndex )const;
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		Device const & getDevice()const noexcept
		{
			return m_device;
		}

		ImagePtrArray const & getImages()const noexcept
		{
			return m_images;
		}

		uint32_t getImageCount()const noexcept
		{
			return m_createInfo.minImageCount;
		}

		VkExtent2D getDimensions()const noexcept
		{
			return m_createInfo.imageExtent;
		}

		VkPresentModeKHR getPresentMode()const noexcept
		{
			return m_createInfo.presentMode;
		}

		VkFormat getFormat()const noexcept
		{
			return m_createInfo.imageFormat;
		}
		/**@}*/
		/**
		*\brief
		*	VkSwapchainKHR implicit cast operator.
		*/
		operator VkSwapchainKHR const & ()const noexcept
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkSwapchainCreateInfoKHR m_createInfo;
		VkSwapchainKHR m_internal{};
		ImagePtrArray m_images;
	};
}

#endif
