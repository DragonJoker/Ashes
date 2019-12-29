/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Validator_HPP___
#define ___GlRenderer_Validator_HPP___
#pragma once

#include "renderer/Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
	InputsLayout getInputs( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	ConstantsLayout getPushConstants( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	InterfaceBlocksLayout getUniformBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	InterfaceBlocksLayout getStorageBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	SamplersLayout getSamplers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	ImagesLayout getImages( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );

	inline ShaderDesc getShaderDesc( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return
		{
			true,
			0u,
			0u,
			getInputs( context, stage, program ),
			getPushConstants( context, stage, program ),
			getUniformBuffers( context, stage, program ),
			getStorageBuffers( context, stage, program ),
			getSamplers( context, stage, program ),
			getImages( context, stage, program ),
		};
	}

	void validatePipeline( ContextLock const & context
		, VkPipelineLayout layout
		, GLuint program
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState
		, VkRenderPass renderPass );
}

#endif
