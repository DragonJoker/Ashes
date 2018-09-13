/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Validator_HPP___
#define ___GlRenderer_Validator_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	void validatePipeline( ContextLock const & context
		, PipelineLayout const & layout
		, GLuint program
		, ashes::VertexInputState const & vertexInputState
		, ashes::RenderPass const & renderPass );
}

#endif
