/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Texture_HPP___
#define ___GlRenderer_Texture_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Enum/GlTextureType.hpp"

namespace ashes::gl
{
	class Image
		: public IcdObject
	{
	public:
		Image( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, bool swapchainImage = false );
		Image( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImageCreateInfo createInfo
			, bool swapchainImage = false );
		~Image();

		VkMemoryRequirements getMemoryRequirements()const;
		std::vector< VkSparseImageMemoryRequirements > getSparseImageMemoryRequirements()const;

		inline VkDevice getDevice()const noexcept
		{
			return m_device;
		}
		
		inline GlTextureType getTarget()const noexcept
		{
			return m_target;
		}

		inline VkImageCreateFlags getCreateFlags()const noexcept
		{
			return m_flags;
		}

		inline VkImageType getType()const noexcept
		{
			return m_imageType;
		}

		inline VkFormat getFormat()const noexcept
		{
			return m_format;
		}

		inline uint32_t getArrayLayers()const noexcept
		{
			return m_arrayLayers;
		}

		inline VkImageUsageFlags getUsage()const noexcept
		{
			return m_usage;
		}

		inline VkSampleCountFlagBits getSamples()const noexcept
		{
			return m_samples;
		}

		inline VkExtent3D const & getDimensions()const noexcept
		{
			return m_extent;
		}

		inline uint32_t getMipLevels()const noexcept
		{
			return m_mipLevels;
		}

		inline bool isSwapchainImage()const noexcept
		{
			return m_swapchainImage;
		}

		inline void setMemory( VkDeviceMemory memory )
		{
			m_memory = memory;
		}

		inline VkDeviceMemory getMemory()const
		{
			return m_memory;
		}

	private:
		void doInitialiseMemoryRequirements();

	private:
		VkDevice m_device;
		VkImageCreateFlags m_flags;
		VkImageType m_imageType;
		VkFormat m_format;
		VkExtent3D m_extent;
		uint32_t m_mipLevels;
		uint32_t m_arrayLayers;
		VkSampleCountFlagBits m_samples;
		VkImageTiling m_tiling;
		VkImageUsageFlags m_usage;
		VkSharingMode m_sharingMode;
		UInt32Array m_queueFamilyIndices;
		GlTextureType m_target;
		bool m_swapchainImage{ false };
		VkDeviceMemory m_memory{ nullptr };
		VkMemoryRequirements m_memoryRequirements;
	};
}

#endif
