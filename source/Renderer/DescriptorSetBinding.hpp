/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_DescriptorSetBinding_HPP___
#define ___Renderer_DescriptorSetBinding_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\brief
	*	Attache de type sampler + texture.
	*/
	class CombinedTextureSamplerBinding
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*\param[in] sampler
		*	L'échantillonneur.
		*/
		CombinedTextureSamplerBinding( DescriptorSetLayoutBinding const & layoutBinding
			, Texture const & view
			, Sampler const & sampler )
			: m_binding{ layoutBinding }
			, m_view{ view }
			, m_sampler{ sampler }
		{
		}
		/**
		*\return
		*	L'attache.
		*/
		inline DescriptorSetLayoutBinding const & getBinding()const
		{
			return m_binding;
		}
		/**
		*\return
		*	L'image.
		*/
		inline Texture const & getTexture()const
		{
			return m_view;
		}
		/**
		*\return
		*	L'échantillonneur.
		*/
		inline Sampler const & getSampler()const
		{
			return m_sampler;
		}

	private:
		DescriptorSetLayoutBinding const & m_binding;
		Texture const & m_view;
		Sampler const & m_sampler;
	};
	/**
	*\brief
	*	Attache de type texture échantillonnée.
	*/
	class SampledTextureBinding
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*/
		SampledTextureBinding( DescriptorSetLayoutBinding const & layoutBinding
			, Texture const & view )
			: m_binding{ layoutBinding }
			, m_view{ view }
		{
		}
		/**
		*\return
		*	L'attache.
		*/
		inline DescriptorSetLayoutBinding const & getBinding()const
		{
			return m_binding;
		}
		/**
		*\return
		*	L'image.
		*/
		inline Texture const & getTexture()const
		{
			return m_view;
		}

	private:
		DescriptorSetLayoutBinding const & m_binding;
		Texture const & m_view;
	};
	/**
	*\brief
	*	Attache de type tampon de variables uniformes.
	*/
	class UniformBufferBinding
	{
	public:
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
		UniformBufferBinding( DescriptorSetLayoutBinding const & layoutBinding
			, UniformBufferBase const & uniformBuffer
			, uint32_t offset )
			: m_binding{ layoutBinding }
			, m_uniformBuffer{ uniformBuffer }
			, m_offset{ offset }
		{
		}
		/**
		*\return
		*	L'attache.
		*/
		inline DescriptorSetLayoutBinding const & getBinding()const
		{
			return m_binding;
		}
		/**
		*\return
		*	Le tampon d'uniformes.
		*/
		inline UniformBufferBase const & getUniformBuffer()const
		{
			return m_uniformBuffer;
		}
		/**
		*\return
		*	L'offset.
		*/
		inline uint32_t getOffset()const
		{
			return m_offset;
		}

	private:
		DescriptorSetLayoutBinding const & m_binding;
		UniformBufferBase const & m_uniformBuffer;
		uint32_t m_offset;
	};
}

#endif
