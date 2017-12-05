/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/PipelineLayout.hpp>
#include <Renderer/PipelineLayout.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Wrapper de vk::PipelineLayout.
	*/
	class PipelineLayout
		: public renderer::PipelineLayout
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout des descripteurs du pipeline.
		*/
		PipelineLayout( renderer::Device const & device
			, renderer::DescriptorSetLayout const * layout );
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPipelineLayout.
		*\~english
		*\brief
		*	VkPipelineLayout implicit cast operator.
		*/
		inline vk::PipelineLayout const & getLayout()const
		{
			return *m_layout;
		}

	private:
		vk::PipelineLayoutPtr m_layout;
	};
}
