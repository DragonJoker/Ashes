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

namespace ashes::D3D11_NAMESPACE
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
			, VkExtent2D const & dimensions
			, ID3D11Texture2D * image );
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, VkImageUsageFlags usageFlags
			, VkImageTiling tiling );
		~Image()noexcept = default;

		VkMemoryRequirements getMemoryRequirements()const;
		std::vector< VkSparseImageMemoryRequirements > getSparseImageMemoryRequirements()const;
		void generateMipmaps( VkCommandBuffer commandBuffer )const;
		VkResult bindMemory( VkDeviceMemory memory
			, VkDeviceSize memoryOffset );
		bool isMapped()const;

		ID3D11Resource * getResource()const noexcept
		{
			return m_image.resource;
		}

		ID3D11Texture1D * getTexture1D()const noexcept
		{
			assert( getType() == VK_IMAGE_TYPE_1D );
			return m_image.tex1D;
		}

		ID3D11Texture2D * getTexture2D()const noexcept
		{
			assert( getType() == VK_IMAGE_TYPE_2D );
			return m_image.tex2D;
		}

		ID3D11Texture3D * getTexture3D()const noexcept
		{
			assert( getType() == VK_IMAGE_TYPE_3D );
			return m_image.tex3D;
		}


		bool isRenderTarget()const noexcept
		{
			return ashes::D3D11_NAMESPACE::isRenderTarget( m_createInfo.usage );
		}

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
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_SAMPLED_BIT )
				|| checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT );
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

		VkDeviceSize getMemoryAlignment()const noexcept
		{
			return m_memoryRequirements.alignment;
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
		
		VkDevice getDevice()const noexcept
		{
			return m_device;
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
		} m_image{};
		VkDeviceMemory m_memory{};
		VkDeviceSize m_memoryOffset{ 0u };
		ObjectMemory * m_objectMemory{ nullptr };
		std::string m_debugName;
		VkMemoryRequirements m_memoryRequirements{};
	};
}

#endif
