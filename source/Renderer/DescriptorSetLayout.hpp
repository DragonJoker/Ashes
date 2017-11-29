/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_DescriptorSetLayout_HPP___
#define ___Renderer_DescriptorSetLayout_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/DescriptorLayout.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::DescriptorLayout.
	*/
	class DescriptorSetLayout
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		DescriptorSetLayout( Device const & device
			, DescriptorSetLayoutBindingArray const & bindings );
		/**
		*\return
		*	L'attache de descripteur au point d'attache donné.
		*/
		DescriptorSetLayoutBinding const & getBinding( uint32_t point )const;
		/**
		*\return
		*	Le descriptor layout vulkan.
		*/
		inline vk::DescriptorLayout const & getLayout()const
		{
			return *m_layout;
		}

	private:
		vk::DescriptorLayoutPtr m_layout;
		DescriptorSetLayoutBindingArray m_bindings;
	};
}

#endif
