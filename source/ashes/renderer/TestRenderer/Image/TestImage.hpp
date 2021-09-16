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
			, VkDeviceSize memoryOffset );
		bool isMapped()const;

		inline uint32_t getMipmapLevels()const
		{
			return m_createInfo.mipLevels;
		}

		inline uint32_t getLayerCount()const
		{
			return m_createInfo.arrayLayers;
		}

		inline bool isSamplable()const
		{
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_SAMPLED_BIT );
		}

		inline bool isStorage()const
		{
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_STORAGE_BIT );
		}

		inline VkDeviceMemory getMemory()const
		{
			assert( m_memory != nullptr );
			return m_memory;
		}

		inline VkDeviceSize getMemoryOffset()const
		{
			return m_memoryOffset;
		}

		inline VkImageCreateInfo const & getCreateInfo()const
		{
			return m_createInfo;
		}

		inline VkExtent3D const & getDimensions()const
		{
			return m_createInfo.extent;
		}

		inline VkImageUsageFlags getUsage()const noexcept
		{
			return m_createInfo.usage;
		}

		inline VkFormat getFormat()const
		{
			return m_createInfo.format;
		}

		inline VkSampleCountFlagBits getSamplesCount()const
		{
			return m_createInfo.samples;
		}

		inline VkImageType getType()const
		{
			return m_createInfo.imageType;
		}

		inline ObjectMemory const & getObjectMemory()const
		{
			assert( m_objectMemory != nullptr );
			return *m_objectMemory;
		}

		inline void setDebugName( std::string name )
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
