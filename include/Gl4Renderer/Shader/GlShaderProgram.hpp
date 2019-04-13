/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
	class ShaderProgram
	{
	public:
		ShaderProgram( VkDevice device
			, VkPipelineShaderStageCreateInfoArray stages
			, bool isRtot );
		ShaderProgram( VkDevice device
			, VkPipelineShaderStageCreateInfo const & stage );
		~ShaderProgram();
		ShaderDesc link( ContextLock const & context )const;

		inline GLuint getProgram()const
		{
			return m_program;
		}

	private:
		VkDevice m_device;
		GLuint m_program;
		VkPipelineShaderStageCreateInfoArray m_stages;
		VkShaderStageFlags m_stageFlags;
		UInt32Array m_shaders;
	};
}
