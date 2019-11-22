/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Texture_HPP___
#define ___D3D11Renderer_Texture_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Image
	{
	public:
		Image( VkImage ) = delete;
		Image & operator=( VkImage ) = delete;
		Image( Image && rhs );
		Image & operator=( Image && rhs );

		Image( VkDevice device
			, VkImageCreateInfo createInfo );
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, ID3D11Texture2D * image );
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, VkImageUsageFlags usageFlags
			, VkImageTiling tiling
			, VkMemoryPropertyFlags memoryFlags );
		~Image();

		VkMemoryRequirements getMemoryRequirements()const;
		std::vector< VkSparseImageMemoryRequirements > getSparseImageMemoryRequirements()const;
		void generateMipmaps( VkCommandBuffer commandBuffer )const;
		VkResult bindMemory( VkDeviceMemory memory
			, VkDeviceSize memoryOffset );
		bool isMapped()const;

		inline ID3D11Resource * getResource()const
		{
			return m_image.resource;
		}

		inline ID3D11Texture1D * getTexture1D()const
		{
			assert( getType() == VK_IMAGE_TYPE_1D );
			return m_image.tex1D;
		}

		inline ID3D11Texture2D * getTexture2D()const
		{
			assert( getType() == VK_IMAGE_TYPE_2D );
			return m_image.tex2D;
		}

		inline ID3D11Texture3D * getTexture3D()const
		{
			assert( getType() == VK_IMAGE_TYPE_3D );
			return m_image.tex3D;
		}


		inline bool isRenderTarget()const
		{
			return ashes::d3d11::isRenderTarget( m_createInfo.usage );
		}

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
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_SAMPLED_BIT )
				|| checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT );
		}

		inline bool isStorage()const
		{
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_STORAGE_BIT );
		}

		inline VkDeviceMemory getMemory()const
		{
			assert( m_memory != VK_NULL_HANDLE );
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
		void doInitialiseMemoryRequirements();

	private:
		VkDevice m_device;
		VkImageCreateInfo m_createInfo;
		union
		{
			ID3D11Resource * resource;
			ID3D11Texture1D * tex1D;
			ID3D11Texture2D * tex2D;
			ID3D11Texture3D * tex3D;
		} m_image;
		VkDeviceMemory m_memory{ VK_NULL_HANDLE };
		VkDeviceSize m_memoryOffset{ 0u };
		ObjectMemory * m_objectMemory{ nullptr };
		std::string m_debugName;
		VkMemoryRequirements m_memoryRequirements;
	};
}

#endif
