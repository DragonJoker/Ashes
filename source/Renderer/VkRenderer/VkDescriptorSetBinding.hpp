/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Renderer/DescriptorSetBinding.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Attache de type sampler + texture.
	*/
	class CombinedTextureSamplerBinding
		: public renderer::CombinedTextureSamplerBinding
	{
	public:
		CombinedTextureSamplerBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, DescriptorSet const & descriptorSet
			, Texture const & texture
			, Sampler const & sampler );
		inline operator VkWriteDescriptorSet const &( )const
		{
			return m_write;
		}

	private:
		TextureView const & m_view;
		Sampler const & m_sampler;
		VkDescriptorImageInfo m_info;
		VkWriteDescriptorSet m_write;
	};
	/**
	*\brief
	*	Attache de type texture échantillonnée.
	*/
	class SampledTextureBinding
		: public renderer::SampledTextureBinding
	{
	public:
		SampledTextureBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, DescriptorSet const & descriptorSet
			, Texture const & texture );
		inline operator VkWriteDescriptorSet const &( )const
		{
			return m_write;
		}

	private:
		TextureView const & m_view;
		VkDescriptorImageInfo m_info;
		VkWriteDescriptorSet m_write;
	};
	/**
	*\brief
	*	Attache de type tampon de variables uniformes.
	*/
	class UniformBufferBinding
		: public renderer::UniformBufferBinding
	{
	public:
		UniformBufferBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, DescriptorSet const & descriptorSet
			, renderer::UniformBufferBase const & uniformBuffer
			, uint32_t offset );
		inline operator VkWriteDescriptorSet const &( )const
		{
			return m_write;
		}

	private:
		Buffer const & m_uniformBuffer;
		VkDescriptorBufferInfo m_info;
		VkWriteDescriptorSet m_write;
	};
}
