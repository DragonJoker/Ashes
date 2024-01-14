/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_DescriptorSet_HPP___
#define ___TestRenderer_DescriptorSet_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

#include <vector>

namespace ashes::test
{
	class DescriptorSet
	{
	public:
		DescriptorSet( VkDevice device
			, VkDescriptorPool pool
			, VkDescriptorSetLayout layout );

		void update( VkWriteDescriptorSet const & write );
		void update( VkCopyDescriptorSet const & write );

		VkDescriptorSetLayout getLayout()const noexcept
		{
			return m_layout;
		}

		LayoutBindingWritesArray const & getCombinedTextureSamplers()const noexcept
		{
			return m_combinedTextureSamplers;
		}

		LayoutBindingWritesArray const & getSamplers()const noexcept
		{
			return m_samplers;
		}

		LayoutBindingWritesArray const & getSampledTextures()const noexcept
		{
			return m_sampledTextures;
		}

		LayoutBindingWritesArray const & getStorageTextures()const noexcept
		{
			return m_storageTextures;
		}

		LayoutBindingWritesArray const & getUniformBuffers()const noexcept
		{
			return m_uniformBuffers;
		}

		LayoutBindingWritesArray const & getStorageBuffers()const noexcept
		{
			return m_storageBuffers;
		}

		LayoutBindingWritesArray const & getTexelBuffers()const noexcept
		{
			return m_texelBuffers;
		}

		LayoutBindingWritesArray const & getDynamicBuffers()const noexcept
		{
			return m_dynamicBuffers;
		}

		LayoutBindingWritesArray const & getDynamicStorageBuffers()const noexcept
		{
			return m_dynamicStorageBuffers;
		}

	private:
		void mergeWrites( LayoutBindingWrites & writes, VkWriteDescriptorSet const & write );

	private:
		VkDescriptorSetLayout m_layout;
		std::vector< std::vector< VkDescriptorImageInfo > > m_imagesInfos;
		std::vector< std::vector< VkDescriptorBufferInfo > > m_buffersInfos;
		LayoutBindingWritesMap m_writes;
		LayoutBindingWritesArray m_combinedTextureSamplers;
		LayoutBindingWritesArray m_samplers;
		LayoutBindingWritesArray m_sampledTextures;
		LayoutBindingWritesArray m_storageTextures;
		LayoutBindingWritesArray m_uniformBuffers;
		LayoutBindingWritesArray m_storageBuffers;
		LayoutBindingWritesArray m_texelBuffers;
		LayoutBindingWritesArray m_dynamicUniformBuffers;
		LayoutBindingWritesArray m_dynamicStorageBuffers;
		LayoutBindingWritesArray m_dynamicBuffers;
	};
}

#endif
