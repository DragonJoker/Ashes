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
	*	Classe de base des attaches.
	*/
	class DescriptorSetBinding
	{
	public:
		virtual ~DescriptorSetBinding() = default;
		inline operator VkWriteDescriptorSet const &()const
		{
			return m_write;
		}

	protected:
		VkWriteDescriptorSet m_write;
	};
	/**
	*\brief
	*	Attache de type sampler + texture.
	*/
	class CombinedTextureSamplerBinding
		: public renderer::CombinedTextureSamplerBinding
		, public DescriptorSetBinding
	{
	public:
		CombinedTextureSamplerBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, DescriptorSet const & descriptorSet
			, Texture const & texture
			, Sampler const & sampler );

	private:
		TextureView const & m_view;
		Sampler const & m_sampler;
		VkDescriptorImageInfo m_info;
	};
	/**
	*\brief
	*	Attache de type texture échantillonnée.
	*/
	class SampledTextureBinding
		: public renderer::SampledTextureBinding
		, public DescriptorSetBinding
	{
	public:
		SampledTextureBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, DescriptorSet const & descriptorSet
			, Texture const & texture );

	private:
		TextureView const & m_view;
		VkDescriptorImageInfo m_info;
	};
	/**
	*\brief
	*	Attache de type tampon de variables uniformes.
	*/
	class UniformBufferBinding
		: public renderer::UniformBufferBinding
		, public DescriptorSetBinding
	{
	public:
		UniformBufferBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, DescriptorSet const & descriptorSet
			, renderer::UniformBufferBase const & uniformBuffer
			, uint32_t offset );

	private:
		Buffer const & m_uniformBuffer;
		VkDescriptorBufferInfo m_info;
	};
}
