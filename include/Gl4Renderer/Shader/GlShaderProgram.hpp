/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace gl_renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram( Device const & device
			, std::vector< ashes::ShaderStageState > const & stages );
		ShaderProgram( Device const & device
			, ashes::ShaderStageState const & stage );
		~ShaderProgram();
		ShaderDesc link( ContextLock const & context )const;

		inline GLuint getProgram()const
		{
			return m_program;
		}

	private:
		Device const & m_device;
		GLuint m_program;
		ashes::ShaderStageFlags m_stageFlags;
		ashes::UInt32Array m_shaders;
	};
}
