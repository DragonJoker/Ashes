/**
*\file
*	GlDescriptorSet.hpp
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSet_HPP___
#define ___GlRenderer_DescriptorSet_HPP___
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

#include <renderer/RendererCommon/InlineUniformBlocks.hpp>

#include <map>
#include <vector>

namespace ashes::gl4
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
		DescriptorSet( VkDescriptorPool pool
			, VkDescriptorSetLayout layout );
		~DescriptorSet();

		void update( VkWriteDescriptorSet const & write );
		void update( VkCopyDescriptorSet const & write );

		inline LayoutBindingWritesArray const & getInputAttachments()const
		{
			return m_inputAttachments;
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

		inline LayoutBindingWritesArray const & getInlineUniforms()const
		{
			return m_inlineUniforms;
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

		inline VkDescriptorSetLayout getLayout()const
		{
			return m_layout;
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
		LayoutBindingWritesArray m_texelBuffers;
		LayoutBindingWritesArray m_dynamicUniformBuffers;
		LayoutBindingWritesArray m_dynamicStorageBuffers;
		LayoutBindingWritesArray m_dynamicBuffers;
		LayoutBindingWritesArray m_inputAttachments;
	};
}

#endif
