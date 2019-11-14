/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Validator_HPP___
#define ___GlRenderer_Validator_HPP___
#pragma once

#include "renderer/Gl3Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl3
{
	InterfaceBlockLayout getInterfaceBlockLayout( ContextLock const & context
		, GLuint program );
	InputLayout getInputLayout( ContextLock const & context
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
		, VkPipelineLayout layout
		, GLuint program
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState
		, VkRenderPass renderPass );
}

#endif
