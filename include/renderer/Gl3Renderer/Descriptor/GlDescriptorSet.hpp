/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSet_HPP___
#define ___GlRenderer_DescriptorSet_HPP___
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorSet.hpp>

#include <vector>

namespace gl_renderer
{
	/**
	*\brief
	*	Set de descripteurs.
	*/
	class DescriptorSet
		: public ashes::DescriptorSet
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
		DescriptorSet( ashes::DescriptorPool const & pool
			, ashes::DescriptorSetLayout const & layout
			, uint32_t bindingPoint );
		/**
		*\copydoc		ashes::DescriptorSet::update
		*/
		void update()const override;
		/**
		*\brief
		*	Le tableau d'attaches de type sampler + texture.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type sampler.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getSamplers()const
		{
			return m_samplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture échantillonnée.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getSampledTextures()const
		{
			return m_sampledTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture de stockage.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getStorageTextures()const
		{
			return m_storageTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon de stockage.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getStorageBuffers()const
		{
			return m_storageBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme de texels.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getTexelBuffers()const
		{
			return m_texelBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon dynamique.
		*/
		inline ashes::VkWriteDescriptorSetArray const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

	private:
		mutable ashes::VkWriteDescriptorSetArray m_combinedTextureSamplers;
		mutable ashes::VkWriteDescriptorSetArray m_samplers;
		mutable ashes::VkWriteDescriptorSetArray m_sampledTextures;
		mutable ashes::VkWriteDescriptorSetArray m_storageTextures;
		mutable ashes::VkWriteDescriptorSetArray m_uniformBuffers;
		mutable ashes::VkWriteDescriptorSetArray m_storageBuffers;
		mutable ashes::VkWriteDescriptorSetArray m_texelBuffers;
		mutable ashes::VkWriteDescriptorSetArray m_dynamicUniformBuffers;
		mutable ashes::VkWriteDescriptorSetArray m_dynamicStorageBuffers;
		mutable ashes::VkWriteDescriptorSetArray m_dynamicBuffers;
	};
}

#endif
