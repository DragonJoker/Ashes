/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Validator_HPP___
#define ___GlRenderer_Validator_HPP___
#pragma once

#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl
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
	SamplersLayout getTextureBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	SamplersLayout getSamplers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	ImagesLayout getImages( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	ShaderDesc getShaderDesc( ContextLock const & context
		, ConstantsLayout const & constants
		, VkShaderStageFlagBits stage
		, GLuint program
		, bool separable );

	void validatePipeline( ContextLock const & context
		, VkPipelineLayout layout
		, GLuint program
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState
		, VkRenderPass renderPass );
}

#endif
