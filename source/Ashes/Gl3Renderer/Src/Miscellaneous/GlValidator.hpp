/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Validator_HPP___
#define ___GlRenderer_Validator_HPP___
#pragma once

#include "Shader/GlShaderDesc.hpp"

namespace gl_renderer
{
	InputLayout getInputLayout( ContextLock const & context
		, GLuint program );
	InterfaceBlockLayout getInterfaceBlockLayout( ContextLock const & context
		, GLuint program );
	ConstantsLayout getConstantsLayout( ContextLock const & context
		, GLuint program );
	inline ShaderDesc getShaderDesc( ContextLock const & context
		, GLuint program )
	{
		return
		{
			0u,
			getInputLayout( context, program ),
			getConstantsLayout( context, program ),
			getInterfaceBlockLayout( context, program )
		};
	}

	void validatePipeline( ContextLock const & context
		, PipelineLayout const & layout
		, GLuint program
		, ashes::VertexInputState const & vertexInputState
		, ashes::RenderPass const & renderPass );
}

#endif
