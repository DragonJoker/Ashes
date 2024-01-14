/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Texture_HPP___
#define ___TestRenderer_Texture_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class Image
	{
	public:
		Image( VkImage ) = delete;
		Image & operator=( VkImage ) = delete;
		Image( Image && rhs )noexcept;
		Image & operator=( Image && rhs )noexcept;
		~Image()noexcept = default;

		Image( VkDevice device
			, VkImageCreateInfo createInfo );
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions );
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, VkImageUsageFlags usageFlags
			, VkImageTiling tiling
			, VkMemoryPropertyFlags memoryFlags );

		VkMemoryRequirements getMemoryRequirements()const;
		std::vector< VkSparseImageMemoryRequirements > getSparseImageMemoryRequirements()const;
		void generateMipmaps( VkCommandBuffer commandBuffer )const;
		VkResult bindMemory( VkDeviceMemory memory
			, VkDeviceSize memoryOffset )noexcept;
		bool isMapped()const;

		uint32_t getMipmapLevels()const noexcept
		{
			return m_createInfo.mipLevels;
		}

		uint32_t getLayerCount()const noexcept
		{
			return m_createInfo.arrayLayers;
		}

		bool isSamplable()const noexcept
		{
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_SAMPLED_BIT );
		}

		bool isStorage()const noexcept
		{
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_STORAGE_BIT );
		}

		VkDeviceMemory getMemory()const noexcept
		{
			assert( m_memory != nullptr );
			return m_memory;
		}

		VkDeviceSize getMemoryOffset()const noexcept
		{
			return m_memoryOffset;
		}

		VkImageCreateInfo const & getCreateInfo()const noexcept
		{
			return m_createInfo;
		}

		VkExtent3D const & getDimensions()const noexcept
		{
			return m_createInfo.extent;
		}

		VkImageUsageFlags getUsage()const noexcept
		{
			return m_createInfo.usage;
		}

		VkFormat getFormat()const noexcept
		{
			return m_createInfo.format;
		}

		VkSampleCountFlagBits getSamplesCount()const noexcept
		{
			return m_createInfo.samples;
		}

		VkImageType getType()const noexcept
		{
			return m_createInfo.imageType;
		}

		ObjectMemory const & getObjectMemory()const noexcept
		{
			assert( m_objectMemory != nullptr );
			return *m_objectMemory;
		}

		void setDebugName( std::string name )noexcept
		{
			m_debugName = std::move( name );
		}

	private:
		VkDevice m_device;
		VkImageCreateInfo m_createInfo;
		VkDeviceMemory m_memory{};
		VkDeviceSize m_memoryOffset{ 0u };
		ObjectMemory * m_objectMemory{ nullptr };
		std::string m_debugName;
	};
}

#endif
