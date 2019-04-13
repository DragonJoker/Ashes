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
	/**
	*\brief
	*	Set de descripteurs.
	*/
	class DescriptorSet
	{
	private:
	public:
		DescriptorSet( VkDescriptorPool pool
			, VkDescriptorSetLayout layout );
		/**
		*\copydoc		DescriptorSet::update
		*/
		void update( VkWriteDescriptorSet const & write );
		/**
		*\copydoc		DescriptorSet::update
		*/
		void update( VkCopyDescriptorSet const & write );
		/**
		*\brief
		*	Le tableau d'attaches de type sampler + get( texture )->
		*/
		inline LayoutBindingWritesArray const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type sampler.
		*/
		inline LayoutBindingWritesArray const & getSamplers()const
		{
			return m_samplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture échantillonnée.
		*/
		inline LayoutBindingWritesArray const & getSampledTextures()const
		{
			return m_sampledTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture de stockage.
		*/
		inline LayoutBindingWritesArray const & getStorageTextures()const
		{
			return m_storageTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme.
		*/
		inline LayoutBindingWritesArray const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon de stockage.
		*/
		inline LayoutBindingWritesArray const & getStorageBuffers()const
		{
			return m_storageBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon uniforme de texels.
		*/
		inline LayoutBindingWritesArray const & getTexelBuffers()const
		{
			return m_texelBuffers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon dynamique.
		*/
		inline LayoutBindingWritesArray const & getDynamicBuffers()const
		{
			return m_dynamicBuffers;
		}

	private:
		void mergeWrites( LayoutBindingWrites & writes, VkWriteDescriptorSet const & write );

	private:
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
