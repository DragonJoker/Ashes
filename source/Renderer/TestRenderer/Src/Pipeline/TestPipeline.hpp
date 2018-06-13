/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Pipeline_HPP___
#define ___TestRenderer_Pipeline_HPP___
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Pipeline/Pipeline.hpp>

namespace test_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
		: public renderer::Pipeline
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( Device const & device
			, renderer::PipelineLayout const & layout
			, renderer::GraphicsPipelineCreateInfo && createInfo );
		~Pipeline();
	};
}

#endif
