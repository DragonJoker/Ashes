/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DescriptorSet_HPP___
#define ___D3D11Renderer_DescriptorSet_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <vector>

namespace ashes::d3d11
{
	class DescriptorSet
	{
	public:
		DescriptorSet( VkDevice device
			, VkDescriptorPool pool
			, VkDescriptorSetLayout layout );

		void update( VkWriteDescriptorSet const & write );
		void update( VkCopyDescriptorSet const & write );

		inline VkDescriptorSetLayout getLayout()const
		{
			return m_layout;
		}

		inline LayoutBindingWritesArray const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}

		inline LayoutBindingWritesArray const & getSamplers()const
		{
			return m_samplers;
		}

		inline LayoutBindingWritesArray const & getSampledTextures()const
		{
			return m_sampledTextures;
		}

		inline LayoutBindingWritesArray const & getStorageTextures()const
		{
			return m_storageTextures;
		}

		inline LayoutBindingWritesArray const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}

		inline LayoutBindingWritesArray const & getStorageBuffers()const
		{
			return m_storageBuffers;
		}

		inline LayoutBindingWritesArray const & getTexelBuffers()const
		{
			return m_texelBuffers;
		}

		inline LayoutBindingWritesArray const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

		inline LayoutBindingWritesArray const & getDynamicStorageBuffers()const
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
