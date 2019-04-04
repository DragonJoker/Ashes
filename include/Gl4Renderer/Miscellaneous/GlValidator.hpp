/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Validator_HPP___
#define ___GlRenderer_Validator_HPP___
#pragma once

#include "Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
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
