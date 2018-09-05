/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram( Device const & device
			, std::vector< renderer::ShaderStageState > const & stages );
		ShaderProgram( Device const & device
			, renderer::ShaderStageState const & stage );
		~ShaderProgram();
		void link( ContextLock const & context )const;

		inline GLuint getProgram()const
		{
			return m_program;
		}

	private:
		Device const & m_device;
		GLuint m_program;
		renderer::UInt32Array m_shaders;
	};
}
