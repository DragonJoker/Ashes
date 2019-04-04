/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSet_HPP___
#define ___GlRenderer_DescriptorSet_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <vector>

namespace ashes::gl4
{
	/**
	*\brief
	*	Set de descripteurs.
	*/
	class DescriptorSet
	{
	public:
		/**
		*\copydoc		DescriptorSet::update
		*/
		void update( VkWriteDescriptorSet const & write )const;
		/**
		*\copydoc		DescriptorSet::update
		*/
		void update( VkCopyDescriptorSet const & write )const;
		/**
		*\copydoc		DescriptorSet::update
		*/
		void update()const;
		/**
		*\brief
		*	Le tableau d'attaches de type sampler + get( texture )->
		*/
		inline VkWriteDescriptorSetArray const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type sampler.
		*/
		inline VkWriteDescriptorSetArray const & getSamplers()const
		{
			return m_samplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture échantillonnée.
		*/
		inline VkWriteDescriptorSetArray const & getSampledTextures()const
		{
			return m_sampledTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture de stockage.
		*/
		inline VkWriteDescriptorSetArray const & getStorageTextures()const
		{
			return m_storageTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme.
		*/
		inline VkWriteDescriptorSetArray const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon de stockage.
		*/
		inline VkWriteDescriptorSetArray const & getStorageBuffers()const
		{
			return m_storageBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme de texels.
		*/
		inline VkWriteDescriptorSetArray const & getTexelBuffers()const
		{
			return m_texelBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon dynamique.
		*/
		inline VkWriteDescriptorSetArray const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

	private:
		mutable VkWriteDescriptorSetArray m_writes;
		mutable VkWriteDescriptorSetArray m_combinedTextureSamplers;
		mutable VkWriteDescriptorSetArray m_samplers;
		mutable VkWriteDescriptorSetArray m_sampledTextures;
		mutable VkWriteDescriptorSetArray m_storageTextures;
		mutable VkWriteDescriptorSetArray m_uniformBuffers;
		mutable VkWriteDescriptorSetArray m_storageBuffers;
		mutable VkWriteDescriptorSetArray m_texelBuffers;
		mutable VkWriteDescriptorSetArray m_dynamicUniformBuffers;
		mutable VkWriteDescriptorSetArray m_dynamicStorageBuffers;
		mutable VkWriteDescriptorSetArray m_dynamicBuffers;
	};
}

#endif
