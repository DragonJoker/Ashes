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

#include <renderer/RendererCommon/InlineUniformBlocks.hpp>

#include <vector>

namespace ashes::d3d11
{
	class DescriptorSet
	{
	public:
		DescriptorSet( VkDevice device
			, VkDescriptorPool pool
			, VkDescriptorSetLayout layout );
		~DescriptorSet()noexcept;

		void update( VkWriteDescriptorSet const & write );
		void update();

		VkDescriptorSetLayout getLayout()const
		{
			return m_layout;
		}

		LayoutBindingWritesArray const & getInputAttachments()const
		{
			return m_inputAttachments;
		}

		LayoutBindingWritesArray const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}

		LayoutBindingWritesArray const & getSamplers()const
		{
			return m_samplers;
		}

		LayoutBindingWritesArray const & getSampledTextures()const
		{
			return m_sampledTextures;
		}

		LayoutBindingWritesArray const & getStorageTextures()const
		{
			return m_storageTextures;
		}

		LayoutBindingWritesArray const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}

		LayoutBindingWritesArray const & getInlineUniforms()const
		{
			return m_inlineUniforms;
		}

		LayoutBindingWritesArray const & getStorageBuffers()const
		{
			return m_storageBuffers;
		}

		LayoutBindingWritesArray const & getUniformTexelBuffers()const
		{
			return m_uniformTexelBuffers;
		}

		LayoutBindingWritesArray const & getStorageTexelBuffers()const
		{
			return m_storageTexelBuffers;
		}

		LayoutBindingWritesArray const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

		LayoutBindingWritesArray const & getDynamicStorageBuffers()const
		{
			return m_dynamicStorageBuffers;
		}

	private:
		void mergeWrites( LayoutBindingWrites & writes, VkWriteDescriptorSet const & write );

	private:
		VkDescriptorPool m_pool;
		VkDescriptorSetLayout m_layout;
		std::vector< std::vector< VkDescriptorImageInfo > > m_imagesInfos;
		std::vector< std::vector< VkDescriptorBufferInfo > > m_buffersInfos;
		InlineUboArray m_inlineUbos;
		LayoutBindingWritesMap m_writes;
		LayoutBindingWritesArray m_combinedTextureSamplers;
		LayoutBindingWritesArray m_samplers;
		LayoutBindingWritesArray m_sampledTextures;
		LayoutBindingWritesArray m_storageTextures;
		LayoutBindingWritesArray m_uniformBuffers;
		LayoutBindingWritesArray m_inlineUniforms;
		LayoutBindingWritesArray m_storageBuffers;
		LayoutBindingWritesArray m_uniformTexelBuffers;
		LayoutBindingWritesArray m_storageTexelBuffers;
		LayoutBindingWritesArray m_dynamicUniformBuffers;
		LayoutBindingWritesArray m_dynamicStorageBuffers;
		LayoutBindingWritesArray m_dynamicBuffers;
		LayoutBindingWritesArray m_inputAttachments;
	};
}

#endif
