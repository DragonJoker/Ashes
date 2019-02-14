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

#include <Ashes/Pipeline/Pipeline.hpp>

namespace test_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
		: public ashes::Pipeline
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::GraphicsPipelineCreateInfo && createInfo );
		~Pipeline();
	};
}

#endif
