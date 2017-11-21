/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_DescriptorSet_HPP___
#define ___Renderer_DescriptorSet_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/DescriptorSet.hpp>

#include <vector>

namespace renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::DescriptorSet.
	*/
	class DescriptorSet
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] resources
		*	Les ressources de rendu.
		*/
		DescriptorSet( DescriptorSetPool const & pool );
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
		void createBinding( DescriptorSetLayoutBinding const & layoutBinding
			, Texture const & view
			, Sampler const & sampler );
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
		void createBinding( DescriptorSetLayoutBinding const & layoutBinding
			, Texture const & view );
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
		template< typename T >
		void createBinding( DescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer< T > const & uniformBuffer );
		/**
		*\brief
		*	Met à jour toutes les attaches du descripteur.
		*/
		void update()const;
		/**
		*\return
		*	Le descriptor set vulkan.
		*/
		inline vk::DescriptorSet const & getDescriptorSet()const
		{
			return *m_descriptorSet;
		}

	private:
		vk::DescriptorSetPtr m_descriptorSet;
	};
}

#include "DescriptorSet.inl"

#endif
