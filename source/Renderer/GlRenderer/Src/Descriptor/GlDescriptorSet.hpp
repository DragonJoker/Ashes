/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSet_HPP___
#define ___GlRenderer_DescriptorSet_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Descriptor/DescriptorSet.hpp>

#include <vector>

namespace gl_renderer
{
	/**
	*\brief
	*	Set de descripteurs.
	*/
	class DescriptorSet
		: public renderer::DescriptorSet
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] pool
		*	Le pool parent.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] pool
		*	The parent pool.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*/
		DescriptorSet( renderer::DescriptorPool const & pool
			, renderer::DescriptorSetLayout const & layout
			, uint32_t bindingPoint );
		/**
		*\copydoc		renderer::DescriptorSet::createBinding
		*/
		renderer::CombinedTextureSamplerBinding const & createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::TextureView const & view
			, renderer::Sampler const & sampler
			, renderer::ImageLayout layout
			, uint32_t index )override;
		/**
		*\copydoc		renderer::DescriptorSet::createBinding
		*/
		renderer::SamplerBinding const & createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::Sampler const & sampler
			, uint32_t index )override;
		/**
		*\copydoc		renderer::DescriptorSet::createBinding
		*/
		renderer::SampledTextureBinding const & createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::TextureView const & view
			, renderer::ImageLayout layout
			, uint32_t index )override;
		/**
		*\copydoc		renderer::DescriptorSet::createBinding
		*/
		renderer::StorageTextureBinding const & createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::TextureView const & view
			, uint32_t index )override;
		/**
		*\copydoc		renderer::DescriptorSet::createBinding
		*/
		renderer::UniformBufferBinding const & createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::UniformBufferBase const & uniformBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index )override;
		/**
		*\copydoc		renderer::DescriptorSet::createBinding
		*/
		renderer::StorageBufferBinding const & createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::BufferBase const & storageBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index )override;
		/**
		*\copydoc		renderer::DescriptorSet::createBinding
		*/
		renderer::TexelBufferBinding const & createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::BufferBase const & buffer
			, renderer::BufferView const & view
			, uint32_t index )override;
		/**
		*\copydoc	renderer::DescriptorSet::createDynamicBinding
		*/
		renderer::DynamicUniformBufferBinding const & createDynamicBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::UniformBufferBase const & uniformBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u )override;
		/**
		*\copydoc		renderer::DescriptorSet::createDynamicBinding
		*/
		renderer::DynamicStorageBufferBinding const & createDynamicBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::BufferBase const & storageBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index )override;
		/**
		*\copydoc		renderer::DescriptorSet::update
		*/
		void update()const override;
		/**
		*\brief
		*	Le tableau d'attaches de type sampler + texture.
		*/
		inline std::vector< std::unique_ptr< renderer::CombinedTextureSamplerBinding > > const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type sampler.
		*/
		inline std::vector< std::unique_ptr< renderer::SamplerBinding > > const & getSamplers()const
		{
			return m_samplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture échantillonnée.
		*/
		inline std::vector< std::unique_ptr< renderer::SampledTextureBinding > > const & getSampledTextures()const
		{
			return m_sampledTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture de stockage.
		*/
		inline std::vector< std::unique_ptr< renderer::StorageTextureBinding > > const & getStorageTextures()const
		{
			return m_storageTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme.
		*/
		inline std::vector< std::unique_ptr< renderer::UniformBufferBinding > > const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon de stockage.
		*/
		inline std::vector< std::unique_ptr< renderer::StorageBufferBinding > > const & getStorageBuffers()const
		{
			return m_storageBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme de texels.
		*/
		inline std::vector< std::unique_ptr< renderer::TexelBufferBinding > > const & getTexelBuffers()const
		{
			return m_texelBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon dynamique.
		*/
		inline std::vector< std::reference_wrapper< renderer::DescriptorSetBinding > > const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

	private:
		std::vector< std::unique_ptr< renderer::CombinedTextureSamplerBinding > > m_combinedTextureSamplers;
		std::vector< std::unique_ptr< renderer::SamplerBinding > > m_samplers;
		std::vector< std::unique_ptr< renderer::SampledTextureBinding > > m_sampledTextures;
		std::vector< std::unique_ptr< renderer::StorageTextureBinding > > m_storageTextures;
		std::vector< std::unique_ptr< renderer::UniformBufferBinding > > m_uniformBuffers;
		std::vector< std::unique_ptr< renderer::StorageBufferBinding > > m_storageBuffers;
		std::vector< std::unique_ptr< renderer::TexelBufferBinding > > m_texelBuffers;
		std::vector< std::unique_ptr< renderer::DynamicUniformBufferBinding > > m_dynamicUniformBuffers;
		std::vector< std::unique_ptr< renderer::DynamicStorageBufferBinding > > m_dynamicStorageBuffers;
		mutable std::vector< std::reference_wrapper< renderer::DescriptorSetBinding > > m_dynamicBuffers;
	};
}

#endif
