/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_DescriptorSet_HPP___
#define ___VkRenderer_DescriptorSet_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Renderer/DescriptorSet.hpp>

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
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		DescriptorSet( renderer::DescriptorSetPool const & pool );
		/**
		*\brief
		*	Crée une attache de type image et échantillonneur combinés.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*\param[in] sampler
		*	L'échantillonneur.
		*\return
		*	L'attache créée.
		*/
		renderer::CombinedTextureSamplerBinding createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::Texture const & view
			, renderer::Sampler const & sampler );
		/**
		*\brief
		*	Crée une attache de type image échantillonée.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*\return
		*	L'attache créée.
		*/
		renderer::SampledTextureBinding createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::Texture const & view );
		/**
		*\brief
		*	Crée une attache de type tampon de variables uniformes.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] uniformBuffer
		*	Le tampon.
		*\return
		*	L'attache créée.
		*/
		renderer::UniformBufferBinding createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::UniformBufferBase const & uniformBuffer
			, uint32_t offset );
		/**
		*\brief
		*	Met à jour toutes les attaches du descripteur.
		*/
		void update()const override;
		/**
		*\brief
		*	Le tableau d'attaches de type sampler + texture..
		*/
		inline std::vector< renderer::CombinedTextureSamplerBinding > const & getCombinedTextureSamplers()const
		{
			return m_combinedTextureSamplers;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type texture échantillonnée.
		*/
		inline std::vector< renderer::SampledTextureBinding > const & getSampledTextures()const
		{
			return m_sampledTextures;
		}
		/**
		*\brief
		*	Le tableau d'attaches de type tampon de variables uniformes.
		*/
		inline std::vector< renderer::UniformBufferBinding > const & getUniformBuffers()const
		{
			return m_uniformBuffers;
		}

	private:
		std::vector< renderer::CombinedTextureSamplerBinding > m_combinedTextureSamplers;
		std::vector< renderer::SampledTextureBinding > m_sampledTextures;
		std::vector< renderer::UniformBufferBinding > m_uniformBuffers;
	};
}

#endif
