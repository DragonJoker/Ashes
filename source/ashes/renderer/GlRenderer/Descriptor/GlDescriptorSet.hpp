/**
*\file
*	GlDescriptorSet.hpp
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSet_HPP___
#define ___GlRenderer_DescriptorSet_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include <renderer/RendererCommon/InlineUniformBlocks.hpp>

#include <map>
#include <vector>

namespace ashes::gl
{
	struct LayoutBindingWrites
	{
		uint32_t binding;
		uint32_t descriptorCount;
		VkDescriptorType descriptorType;
		VkWriteDescriptorSetArray writes;
	};
	using LayoutBindingWritesArray = std::vector< LayoutBindingWrites * >;
	using LayoutBindingWritesMap = std::map< uint32_t, LayoutBindingWrites >;

	class DescriptorSet
		: public AutoIdIcdObject< DescriptorSet >
	{
	public:
		DescriptorSet( VkAllocationCallbacks const * allocInfo
			, VkDescriptorPool pool
			, VkDescriptorSetLayout layout );
		~DescriptorSet()noexcept;

		void update( VkWriteDescriptorSet const & write );
		void update( VkCopyDescriptorSet const & write );

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

		LayoutBindingWritesArray const & getTexelSamplerBuffers()const
		{
			return m_texelSamplerBuffers;
		}

		LayoutBindingWritesArray const & getTexelImageBuffers()const
		{
			return m_texelImageBuffers;
		}

		LayoutBindingWritesArray const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

		VkDescriptorSetLayout getLayout()const
		{
			return m_layout;
		}

	private:
		void mergeWrites( LayoutBindingWrites & writes, VkWriteDescriptorSet const & write );

	private:
		VkDevice m_device;
		VkAllocationCallbacks const * m_allocInfo;
		VkDescriptorPool m_pool;
		VkDescriptorSetLayout m_layout;
		std::vector< std::vector< VkDescriptorImageInfo > > m_imagesInfos;
		std::vector< std::vector< VkDescriptorBufferInfo > > m_buffersInfos;
		std::vector< std::vector< VkBufferView > > m_texelBufferViews;
		InlineUboArray m_inlineUbos;
		LayoutBindingWritesMap m_writes;
		LayoutBindingWritesArray m_combinedTextureSamplers;
		LayoutBindingWritesArray m_samplers;
		LayoutBindingWritesArray m_sampledTextures;
		LayoutBindingWritesArray m_storageTextures;
		LayoutBindingWritesArray m_uniformBuffers;
		LayoutBindingWritesArray m_inlineUniforms;
		LayoutBindingWritesArray m_storageBuffers;
		LayoutBindingWritesArray m_texelSamplerBuffers;
		LayoutBindingWritesArray m_texelImageBuffers;
		LayoutBindingWritesArray m_dynamicUniformBuffers;
		LayoutBindingWritesArray m_dynamicStorageBuffers;
		LayoutBindingWritesArray m_dynamicBuffers;
		LayoutBindingWritesArray m_inputAttachments;
	};
}

#endif
