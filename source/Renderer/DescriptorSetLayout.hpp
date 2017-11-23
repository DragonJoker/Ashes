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
			, std::vector< DescriptorSetLayoutBinding > const & bindings );
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
	};
}

#endif
