/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Shader/GlShaderDesc.hpp"

#include <renderer/RendererCommon/ShaderBindings.hpp>

namespace ashes::gl
{
	class ShaderProgram
	{
	public:
		ShaderProgram( VkDevice device
			, ContextState const * state
			, VkPipeline pipeline
			, VkPipelineShaderStageCreateInfoArray stages
			, VkPipelineLayout layout
			, VkPipelineCreateFlags createFlags
			, VkRenderPass renderPass
			, Optional< VkPipelineVertexInputStateCreateInfo > const & vertexInputState
			, bool invertY = false );
		~ShaderProgram()noexcept;

	private:
		VkDevice m_device;

	public:
		PushConstantsDesc constantsPcb{};
		UInt32Array modules;
		ShaderDesc program{};
		ShaderBindings bindings{};
		VkPipelineShaderStageCreateInfoArray stages;
		VkShaderStageFlags stageFlags{};

	private:
		void doInitProgramPipeline( ContextLock const & context
			, std::vector< ShaderDesc > const & descs
			, VkPipelineLayout layout
			, VkRenderPass renderPass
			, Optional< VkPipelineVertexInputStateCreateInfo > const & vertexInputState );
		void doInitShaderProgram( ContextLock const & context
			, VkPipeline pipeline
			, std::vector< ShaderDesc > const & descs );
		void doCleanupProgramPipeline( ContextLock const & context )noexcept;
		void doCleanupShaderProgram( ContextLock const & context )noexcept;
	};
}
