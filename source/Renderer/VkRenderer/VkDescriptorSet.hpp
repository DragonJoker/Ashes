/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_DescriptorSet_HPP___
#define ___VkRenderer_DescriptorSet_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/DescriptorSet.hpp>
#include <Renderer/DescriptorSet.hpp>

#include <vector>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::DescriptorSet.
	*/
	class DescriptorSet
		: public renderer::DescriptorSet
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le pool parent.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] pool
		*	The parent pool.
		*/
		DescriptorSet( Device const & device
			, DescriptorSetPool const & pool );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~DescriptorSet();
		/**
		*\~french
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
		*\~english
		*\brief
		*	Creates a combined image and sampler binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] view
		*	The image.
		*\param[in] sampler
		*	The sampler.
		*\return
		*	The created binding.
		*/
		renderer::CombinedTextureSamplerBinding createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::Texture const & view
			, renderer::Sampler const & sampler );
		/**
		*\~french
		*\brief
		*	Crée une attache de type image échantillonée.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*\return
		*	L'attache créée.
		*\~english
		*\brief
		*	Creates a sampled image binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] view
		*	The image.
		*\return
		*	The created binding.
		*/
		renderer::SampledTextureBinding createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::Texture const & view );
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de variables uniformes.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] uniformBuffer
		*	Le tampon.
		*\return
		*	L'attache créée.
		*\~english
		*\brief
		*	Creates a uniform variables buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] uniformBuffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] size
		*	The attach's size in the buffer.
		*\return
		*	The created binding.
		*/
		renderer::UniformBufferBinding createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
			, renderer::UniformBufferBase const & uniformBuffer
			, uint32_t offset );
		/**
		*\~french
		*\brief
		*	Met à jour toutes les attaches du descripteur.
		*\~english
		*\brief
		*	Updates all this descriptor's attaches.
		*/
		void update()const override;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkDescriptorSet.
		*\~english
		*\brief
		*	VkDescriptorSet implicit cast operator.
		*/
		inline operator VkDescriptorSet const &( )const
		{
			return m_descriptorSet;
		}

	private:
		Device const & m_device;
		DescriptorSetPool const & m_pool;
		DescriptorSetLayout const & m_layout;
		VkDescriptorSet m_descriptorSet{};
	};
}

#endif
