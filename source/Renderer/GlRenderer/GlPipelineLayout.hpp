/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Renderer/PipelineLayout.hpp>

namespace gl_renderer
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

	private:
		//vk::PipelineLayoutPtr m_layout;
	};
}
