/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___GlRenderer_ValidatorInterfaceQuery_HPP___
#define ___GlRenderer_ValidatorInterfaceQuery_HPP___
#pragma once

#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl::gl4
{
	void validateInputs( ContextLock const & context
		, GLuint program
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState );
	void validateOutputs( ContextLock const & context
		, GLuint program
		, VkRenderPass renderPass );
	void validateUbos( ContextLock const & context
		, GLuint program );
	void validateSsbos( ContextLock const & context
		, GLuint program );
	void validateUniforms( ContextLock const & context
		, GLuint program );

	InputsLayout getInputs( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	ConstantsLayout & getPushConstants( ContextLock const & context
		, ConstantsLayout & constants
		, VkShaderStageFlagBits stage
		, GLuint program );
	InterfaceBlocksLayout getUniformBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	InterfaceBlocksLayout getStorageBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	SamplersLayout getSamplerBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	SamplersLayout getSamplers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	ImagesLayout getImageBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
	ImagesLayout getImages( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program );
}

#endif
