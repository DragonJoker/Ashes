/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_ComputePipeline_HPP___
#define ___TestRenderer_ComputePipeline_HPP___
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Pipeline/ComputePipeline.hpp>

namespace test_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class ComputePipeline
		: public renderer::ComputePipeline
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] program
		*	Les informations de creation du pipeline.
		*/
		ComputePipeline( Device const & device
			, renderer::PipelineLayout const & layout
			, renderer::ComputePipelineCreateInfo && createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~ComputePipeline();
	};
}

#endif
