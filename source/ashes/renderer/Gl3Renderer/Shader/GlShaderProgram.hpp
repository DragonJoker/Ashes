/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl3Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl3
{
	class ShaderProgram
	{
	public:
		ShaderProgram( VkDevice device
			, VkPipeline pipeline
			, VkPipelineShaderStageCreateInfoArray stages
			, VkPipelineCreateFlags createFlags
			, bool isRtot );
		ShaderProgram( VkDevice device
			, VkPipeline pipeline
			, VkPipelineShaderStageCreateInfo const & stage
			, VkPipelineCreateFlags createFlags );
		~ShaderProgram();
		ShaderDesc link( ContextLock const & context )const;

		inline GLuint getProgram()const
		{
			return m_program;
		}

	private:
		VkDevice m_device;
		VkPipeline m_pipeline;
		GLuint m_program;
		VkPipelineShaderStageCreateInfoArray m_stages;
		VkShaderStageFlags m_stageFlags;
		UInt32Array m_shaders;
	};
}
