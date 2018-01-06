/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_DescriptorSetLayout_HPP___
#define ___VkRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/DescriptorLayout.hpp>
#include <Renderer/DescriptorSetLayout.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::DescriptorLayout.
	*/
	class DescriptorSetLayout
		: public renderer::DescriptorSetLayout
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		DescriptorSetLayout( renderer::Device const & device
			, renderer::DescriptorSetLayoutBindingArray && bindings );
		/**
		*\return
		*	Le descriptor layout vulkan.
		*/
		inline vk::DescriptorLayout const & getLayout()const
		{
			return *m_layout;
		}
		/**
		*\brief
		*	Crée un pool pour les descripteurs qui utiliseront ce layout.
		*\param[in] maxSets
		*	Le nombre maximum de sets que le pool peut créer.
		*\return
		*	Le pool.
		*/
		renderer::DescriptorSetPoolPtr createPool( uint32_t maxSets )const override;

	private:
		vk::DescriptorLayoutPtr m_layout;
	};
}

#endif
