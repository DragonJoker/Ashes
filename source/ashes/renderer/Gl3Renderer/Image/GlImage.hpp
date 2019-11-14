/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Texture_HPP___
#define ___GlRenderer_Texture_HPP___
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl3Renderer/Enum/GlTextureType.hpp"

namespace ashes::gl3
{
	class Image
	{
	public:
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, bool swapchainImage = false );
		Image( VkDevice device
			, VkImageCreateInfo createInfo
			, bool swapchainImage = false );
		~Image();

		VkMemoryRequirements getMemoryRequirements()const;

		inline bool hasInternal()const noexcept
		{
			return m_internal != GL_INVALID_INDEX;
		}

		inline GLuint getInternal()const noexcept
		{
			assert( hasInternal() );
			return m_internal;
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
		GLuint m_internal{ GL_INVALID_INDEX };
		bool m_swapchainImage{ false };
	};
}

#endif
