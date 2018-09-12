/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DescriptorSet_HPP___
#define ___D3D11Renderer_DescriptorSet_HPP___
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/WriteDescriptorSet.hpp>

#include <vector>

namespace d3d11_renderer
{
	class DescriptorSet
		: public renderer::DescriptorSet
	{
	public:
		DescriptorSet( Device const & device
			, DescriptorPool const & pool
			, DescriptorSetLayout const & layout
			, uint32_t bindingPoint );
		~DescriptorSet();

		void update()const override;

		inline WriteDescriptorSetBindingArray const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}

		inline WriteDescriptorSetBindingArray const & getSamplers()const
		{
			return m_samplers;
		}

		inline WriteDescriptorSetBindingArray const & getSampledTextures()const
		{
			return m_sampledTextures;
		}

		inline WriteDescriptorSetBindingArray const & getStorageTextures()const
		{
			return m_storageTextures;
		}

		inline WriteDescriptorSetBindingArray const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}

		inline WriteDescriptorSetBindingArray const & getStorageBuffers()const
		{
			return m_storageBuffers;
		}

		inline WriteDescriptorSetBindingArray const & getTexelBuffers()const
		{
			return m_texelBuffers;
		}

		inline WriteDescriptorSetBindingArray const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

	private:
		DescriptorPool const & m_pool;
		DescriptorSetLayout const & m_layout;
		mutable WriteDescriptorSetBindingArray m_combinedTextureSamplers;
		mutable WriteDescriptorSetBindingArray m_samplers;
		mutable WriteDescriptorSetBindingArray m_sampledTextures;
		mutable WriteDescriptorSetBindingArray m_storageTextures;
		mutable WriteDescriptorSetBindingArray m_uniformBuffers;
		mutable WriteDescriptorSetBindingArray m_storageBuffers;
		mutable WriteDescriptorSetBindingArray m_texelBuffers;
		mutable WriteDescriptorSetBindingArray m_dynamicUniformBuffers;
		mutable WriteDescriptorSetBindingArray m_dynamicStorageBuffers;
		mutable WriteDescriptorSetBindingArray m_dynamicBuffers;
	};
}

#endif
